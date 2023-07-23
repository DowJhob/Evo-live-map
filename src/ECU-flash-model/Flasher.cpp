#include "Flasher.h"

Flasher::Flasher()
{
//    qDebug() << "DMA_proto";
}

Flasher::~Flasher()
{
//    qDebug() << "~ECU_interface";
}

void Flasher::setCommDev(comm_device_interface **devComm)
{
    qDebug() << "=========== FLASH_proto::setCommDev ================" << devComm;

    this->devComm = devComm;
}

bool Flasher::connectMMC()
{
//connect(*devComm, &comm_device_interface::);
    // тут  надо бы проверять что мы отсоединены, наверное

    qDebug() << "=========== MMC_FLASH::connect ================ baudRate" << MMC_FLASH_BAUD;
    if( (*devComm)->open(Protocol::ISO9141, ConnectFlag::ISO9141NoChecksum, MMC_FLASH_BAUD))
    {
        qDebug() << "=========== MMC_FLASH::connect ================ (*devComm)->open";
        if ((*devComm)->connect())
        {
            qDebug() << "=========== MMC_FLASH::connect ================ (*devComm)->connect()";
            if ((*devComm)->setProgLine() )
            {
                QThread::msleep(500);

                *(*devComm)->p_out_buff = 0x100001;  // send init sequence???
                (*devComm)->write(3);
                return true;
            }
        }
    }
    (*devComm)->close();
    return false;
}

QByteArray Flasher::writeFLASH(quint32 addr, int lenght)
{

}

QByteArray Flasher::readFLASH(quint32 addr, int len)
{

}



int Flasher::get_changed_blocks(const uint8_t *src, const uint8_t *orig_data, const struct flashdev_t *fdt, bool *modified) {

    unsigned blockno;

    printf("\n");
    for (blockno = 0; blockno < (fdt->numblocks); blockno++) {
        quint32 bs, blen;
        bs = fdt->fblocks[blockno].start;
        blen = fdt->fblocks[blockno].len;

        printf("\rchecking block %02u/%02u (%06lX-%06lX)...",
               blockno, fdt->numblocks -1, (unsigned long) bs, (unsigned long) bs + blen -1);
        fflush(stdout);
        /* compare with caller's buffer if provided: */
        if (orig_data) {
            if (memcmp(&src[bs], &orig_data[bs], blen) == 0) {
                modified[blockno] = 0;
            } else {
                modified[blockno] = 1;
            }
        } else {
            /* otherwise do CRC comparison with ECU */
            if (check_romcrc(&src[bs], bs, blen, &modified[blockno])) {
                return -1;
            }
        }
    }
    printf(" done.\n");
    return 0;
}





int Flasher::reflash_block(const uint8_t *newdata, const struct flashdev_t *fdt, unsigned blockno, bool practice) {
    uint8_t txdata[64]; //data for nisreq
    struct diag_msg nisreq={0}; //request to send
    int errval;
    struct diag_msg *rxmsg;

    uint32_t start;
    uint32_t len;

    nisreq.data=txdata;

    if (blockno >= fdt->numblocks) {
        printf("block # out of range !\n");
        return -1;
    }

    start = fdt->fblocks[blockno].start;
    len = fdt->fblocks[blockno].len;

    /* 1- requestdownload */
    txdata[0]=SID_FLREQ;
    nisreq.len = 1;
    rxmsg = diag_l2_request(global_l2_conn, &nisreq, &errval);
    if (rxmsg==NULL) {
        goto badexit;
    }
    if (rxmsg->data[0] != (SID_FLREQ + 0x40)) {
        printf("got bad RequestDownload response : %s\n", decode_nrc(rxmsg->data));
        diag_freemsg(rxmsg);
        goto badexit;
    }

    /* 2- Unprotect maybe */
    if (!practice) {
        txdata[0]=SID_FLASH;
        txdata[1]=SIDFL_UNPROTECT;
        txdata[2]=~SIDFL_UNPROTECT;
        nisreq.len = 3;
        rxmsg = diag_l2_request(global_l2_conn, &nisreq, &errval);
        if (rxmsg==NULL) {
            goto badexit;
        }
        if (rxmsg->data[0] != (SID_FLASH + 0x40)) {
            printf("got bad Unprotect response : %s\n", decode_nrc(rxmsg->data));
            diag_freemsg(rxmsg);
            goto badexit;
        }
        printf("Entered flashing_enabled (unprotected) mode\n");
    }

    /* 3- erase block */
    printf("Erasing block %u (0x%06X-0x%06X)...\n",
           blockno, (unsigned) start, (unsigned) start + len - 1);
    txdata[0] = SID_FLASH;
    txdata[1] = SIDFL_EB;
    txdata[2] = blockno;
    nisreq.len = 3;
    /* Problem : erasing can take a lot more than the default P2max for iso14230 */
    uint16_t old_p2max = global_l2_conn->diag_l2_p2max;
    global_l2_conn->diag_l2_p2max = 1800;
    rxmsg = diag_l2_request(global_l2_conn, &nisreq, &errval);
    global_l2_conn->diag_l2_p2max = old_p2max;  //restore p2max; the rest should be OK
    if (rxmsg==NULL) {
        printf("no ERASE_BLOCK response?\n");
        goto badexit;
    }
    if (rxmsg->data[0] != (SID_FLASH + 0x40)) {
        printf("got bad ERASE_BLOCK response : %s\n", decode_nrc(rxmsg->data));
        diag_freemsg(rxmsg);
        goto badexit;
    }

    /* 4- write */
    errval = npk_raw_flashblock(newdata, start, len);
    if (errval) {
        printf("\nReflash error ! Do not panic, do not reset the ECU immediately. The kernel is "
               "most likely still running and receiving commands !\n");
        goto badexit;
    }

    return 0;

badexit:
    return -1;

}


uint8_t *Flasher::load_rom(const char *fname, uint32_t expect_size) {
    FILE *fpl;
    uint8_t *buf;

    if (!fname) {
        return NULL;
    }
    if (!expect_size) {
        return NULL;
    }

    if ((fpl = fopen(fname, "rb"))==NULL) {
        printf("Cannot open %s !\n", fname);
        return NULL;
    }

    u32 file_len = flen(fpl);
    if (file_len != expect_size) {
        printf("error : wrong file length 0x%06lX (wanted 0x%06lX)!\n",
               (unsigned long) file_len, (unsigned long) expect_size);
        goto badexit;
    }

    if (diag_malloc(&buf, file_len)) {
        printf("malloc prob\n");
        goto badexit;
    }

    if (fread(buf, 1, file_len, fpl) != file_len) {
        printf("fread prob !?\n");
        free(buf);
        goto badexit;
    }

    fclose(fpl);
    return buf;

badexit:
    fclose(fpl);
    return NULL;
}


//TODO : merge these into npkern defs ?
#define KSPEED_FROM_BRR(x) ((20 * 1000 * 1000UL) / (32 * ((x) + 1)))
#define BRR_FROM_KSPEED(x) (((20 * 1000 * 1000UL) / (32 * (x))) - 1)

int Flasher::set_kernel_speed(uint16_t kspeed) {
    struct diag_msg nisreq={0}; //request to send
    struct diag_msg *rxmsg=NULL;    //pointer to the reply
    uint8_t txdata[8];  //data for nisreq

    int errval;
    unsigned newdiv;
    float pct_error;

    if (kspeed < KSPEED_FROM_BRR(0xFF)) {
        printf("kspeed value out of bounds !\n");
        return -1;
    }

    newdiv = BRR_FROM_KSPEED(kspeed);

    /* because of rounding, check this too */
    if ((newdiv == 0) || (newdiv > 0xFF)) {
        printf("Illegal BRR value for kspeed %u !\n", kspeed);
        return -1;
    }

    pct_error = 100.0 * (KSPEED_FROM_BRR(newdiv) - kspeed) / kspeed;
    /* faster kernel speeds are more of a problem since its answer come in too fast
     * for the PC */
    if ((pct_error >= 2) || (pct_error <= -4)) {
        printf("BRR is %.1f%% away from requested value; aborting\n", pct_error);
        return -1;
    }

    nisreq.data=txdata;

    //#define SID_CONF_SETSPEED 0x01	/* set comm speed (BRR divisor reg) : <SID_CONF> <SID_CONF_SETSPEED> <new divisor> */
    txdata[0] = SID_CONF;
    txdata[1] = SID_CONF_SETSPEED;
    txdata[2] = newdiv & 0xff;
    nisreq.len=3;

    rxmsg=diag_l2_request(global_l2_conn, &nisreq, &errval);
    if (rxmsg==NULL) {
        return -1;
    }
    if (rxmsg->data[0] != (SID_CONF + 0x40)) {
        printf("got bad SID_CONF response : ");
        diag_data_dump(stdout, rxmsg->data, rxmsg->len);
        printf("\n");
        diag_freemsg(rxmsg);
        return -1;
    }

    //TODO : remove this when npkern doesn't require 25ms of "quiet time" any more
    diag_os_millisleep(25);
    diag_freemsg(rxmsg);
    return 0;

}

const char *Flasher::get_npk_id(void) {
    struct diag_msg nisreq={0}; //request to send
    struct diag_msg *rxmsg=NULL;    //pointer to the reply
    uint8_t txdata[64]; //data for nisreq
    static char npk_id[256];    //version string
    unsigned idlen;

    int errval;

    nisreq.data=txdata;

    txdata[0]=SID_RECUID;
    nisreq.len=1;
    rxmsg=diag_l2_request(global_l2_conn, &nisreq, &errval);
    if (rxmsg==NULL) {
        return NULL;
    }
    if (rxmsg->data[0] != (SID_RECUID + 0x40)) {
        printf("got bad 1A response : %s", decode_nrc(rxmsg->data));
        diag_freemsg(rxmsg);
        return NULL;
    }

    idlen = rxmsg->len;
    if ((idlen <= 1) ||
        (idlen >= (sizeof(npk_id)))) {
        printf("bad length %u for npk version string ! Old kernel maybe?\n", idlen);
        return NULL;
    }

    memcpy(npk_id, rxmsg->data + 1, idlen - 1); //skip 0x5A
    npk_id[idlen]=0;    //null-terminate

    diag_freemsg(rxmsg);
    return npk_id;

}
