#ifndef DMAPROTO_H
#define DMAPROTO_H

#include <QApplication>
#include <QObject>
#include <QDebug>
#include <QElapsedTimer>

#include "ecu/rammut.h"
#include "abstract-memory.h"
#include "comm-device-interface/comm-device-interface.h"

#include "diag.h"
#include "diag_l1.h"
#include "diag_l2.h"

#define MMC_FLASH_BAUD 9600

#define ROMCRC_NUMCHUNKS 4
#define ROMCRC_CHUNKSIZE 256
#define ROMCRC_ITERSIZE (ROMCRC_NUMCHUNKS * ROMCRC_CHUNKSIZE)
#define ROMCRC_LENMASK ((ROMCRC_NUMCHUNKS * ROMCRC_CHUNKSIZE) - 1)  //should look like 0x3FF

//enum class DMAcomand
//{
//    stockWrite,
//    stockRead,
//    stockInfo,
//    directWrite = 0x04,
//    directRead,
//};

class Flasher : public QObject
{
    Q_OBJECT
public:
    comm_device_interface **devComm = nullptr;
//    ramMUT *ramMut;

    Flasher();
    //explicit Flasher(comm_device_interface **devComm = nullptr);
    virtual ~Flasher();

    void setCommDev(comm_device_interface **devComm = nullptr);

    bool connectMMC();

    QByteArray writeFLASH(quint32 addr, int lenght);
    QByteArray readFLASH(quint32 addr, int len);



    /** load ROM with expected size.
     *
     * @return if success: new buffer to be free'd by caller
     */
    uint8_t *load_rom(const char *fname, uint32_t expect_size);

    /** determine which flashblocks are different :
     * @param src: new ROM data
     * @param orig_data: optional, if specified : compared against *src
     * @param modified: (caller-provided) bool array where comparison results are stored
     *
     *
     * return 0 if comparison completed ok
     */
    int get_changed_blocks(const uint8_t *src, const uint8_t *orig_data, const struct flashdev_t *fdt, bool *modified);

    /** reflash a single block.
     * @param newdata : data for the block of interest (not whole ROM)
     * @param practice : if 1, ROM will not be modified
     * ret 0 if ok
     */
    int reflash_block(const uint8_t *newdata, const struct flashdev_t *fdt, unsigned blockno, bool practice);

    /** set kernel comms speed. Caller must then re-send StartComms
     * ret 0 if ok
     */
    int set_kernel_speed(uint16_t kspeed);

    /** Get npkern ID string
     *
     * caller must not free() the string !
     */
    const char *get_npk_id(void);





//public slots:
//    virtual void nop(quint32 addr, char *buf, int lenght) = 0;
//    void nopp(offsetMemory memory);

//    virtual void startLog(ramMUT *_ramMut);
//    virtual void stopLog() = 0;

//private slots:

private:
    /** Decode negative response code into a short error string.
     *
     * rxdata[] must contain at least 3 bytes, "7F <SID> <NRC>"
     * returns a static char * that must not be free'd !
     */
    const char *decode_nrc(uint8_t *rxdata);

    /*** CRC16 implementation adapted from Lammert Bies
     * https://www.lammertbies.nl/comm/info/crc-calculation.html
     *
     *
     */
    #define NPK_CRC16   0xBAAD  //koopman, 2048bits (256B)
    bool crc_tab16_init = 0;
    quint16 crc_tab16[256];



    /** Decode negative response code into a short error string.
     *
     * rxdata[] must contain at least 3 bytes, "7F <SID> <NRC>"
     * returns a static char * that must not be free'd !
     */


    void init_crc16_tab( void ) {
        quint32 i, j;
        quint16 crc, c;

        for (i=0; i<256; i++) {
            crc = 0;
            c   = (quint16) i;

            for (j=0; j<8; j++) {
                if ( (crc ^ c) & 0x0001 ) {
                    crc = ( crc >> 1 ) ^ NPK_CRC16;
                } else {
                    crc =   crc >> 1;
                }
                c = c >> 1;
            }
            crc_tab16[i] = crc;
        }

        crc_tab16_init = 1;

    }  /* init_crc16_tab */


    quint16 crc16(const quint8 *data, quint32 siz) {
        quint16 crc;

        if ( !crc_tab16_init ) {
            init_crc16_tab();
        }

        crc = 0;

        while (siz > 0) {
            quint16 tmp;
            quint8 nextval;

            nextval = *data++;
            tmp =  crc       ^ nextval;
            crc = (crc >> 8) ^ crc_tab16[ tmp & 0xff ];
            siz -= 1;
        }
        return crc;
    }

    /** compare CRC of source data at *src to ROM
     * the area starting at src[0] is compared to the area of ROM
     * starting at <start>, for a total of <len> bytes (rounded up)
     *
     * Caller must have validated parameters
     *
     * @param modified: result of crc check is written to that variable
     * @return 0 if comparison completed correctly
     */
    int check_romcrc(const uint8_t *src, uint32_t start, uint32_t len, bool *modified) {
        uint8_t txdata[4 + (2*ROMCRC_NUMCHUNKS)];   //data for nisreq
        struct diag_msg nisreq={0}; //request to send
        uint8_t rxbuf[10];
        int errval;
        uint16_t chunko;
        //uint16_t maxchunks;

        len = (len + ROMCRC_LENMASK) & ~ROMCRC_LENMASK;

        chunko = start / ROMCRC_CHUNKSIZE;

        //request format : <SID_CONF> <SID_CONF_CKS1> <CNH> <CNL> <CRC0H> <CRC0L> ...<CRC3H> <CRC3L>
        //verify if <CRCH:CRCL> hash is valid for n*256B chunk of the ROM (starting at <CNH:CNL> * 256)
        unsigned txi;
        txdata[0]=SID_CONF;
        txdata[1]=SID_CONF_CKS1;

        nisreq.data=txdata;


        for (; len > 0; len -= ROMCRC_ITERSIZE, chunko += ROMCRC_NUMCHUNKS) {
            txi = 2;
            txdata[txi++] = chunko >> 8;
            txdata[txi++] = chunko & 0xFF;

            //fill the request with n*CRCs
            unsigned chunk_cnt;
            for (chunk_cnt = 0; chunk_cnt < ROMCRC_NUMCHUNKS; chunk_cnt++) {
                u16 chunk_crc = crc16(src, ROMCRC_CHUNKSIZE);
                src += ROMCRC_CHUNKSIZE;
                txdata[txi++] = chunk_crc >> 8;
                txdata[txi++] = chunk_crc & 0xFF;
            }
            nisreq.len = txi;


            //rxmsg=diag_l2_request(global_l2_conn, &nisreq, &errval);
            errval = diag_l2_send(global_l2_conn, &nisreq);
            if (errval) {
                printf("\nl2_send error!\n");
                return -1;
            }

            //responses :	01 <SID_CONF+0x40> <cks> for good CRC
            //				03 7F <SID_CONF> <SID_CONF_CKS1_BADCKS> <cks> for bad CRC
            // anything else is an error that causes abort
            errval = diag_l1_recv(global_l2_conn->diag_link->l2_dl0d, rxbuf, 3, 50);
            if (errval != 3) {
                printf("\nno response @ chunk %X\n", (unsigned) chunko);
                goto badexit;
            }

            if (rxbuf[1] == SID_CONF + 0x40) {
                continue;
            }
            //so, it's a 03 7F <SID_CONF> <NRC> <cks> response. Get remainder of packet
            errval = diag_l1_recv(global_l2_conn->diag_link->l2_dl0d, rxbuf+3, 2, 50);
            if (errval != 2) {
                printf("\nweirdness @ chunk %X\n", (unsigned) chunko);
                goto badexit;
            }

            if ((rxbuf[2] != SID_CONF) || (rxbuf[3] != SID_CONF_CKS1_BADCKS)) {
                printf("\ngot bad SID_FLASH_CKS1 response : ");
                goto badexit;
            }
            //confirmed bad CRC, we can exit
            *modified = 1;
            return 0;
        }   //for

        *modified = 0;
        return 0;

    badexit:
        diag_data_dump(stdout, rxbuf, sizeof(rxbuf));
        printf("\n");
        (void) diag_l2_ioctl(global_l2_conn, DIAG_IOCTL_IFLUSH, NULL);
        return -1;
    }

    /* special checksum for reflash blocks:
     * "one's complement" checksum; if adding causes a carry, add 1 to sum. Slightly better than simple 8bit sum
     */
    uint8_t cks_add8(uint8_t *data, unsigned len) {
        uint16_t sum = 0;
        for (; len; len--, data++) {
            sum += *data;
            if (sum & 0x100) {
                sum += 1;
            }
            sum = (uint8_t) sum;
        }
        return sum;
    }


    /* ret 0 if ok. For use by reflash_block(),
     * assumes parameters have been validated,
     * and appropriate block has been erased
     */

    int npk_raw_flashblock(const uint8_t *src, uint32_t start, uint32_t len) {

        /* program 128-byte chunks */
        uint32_t remain = len;

        uint8_t txdata[134];    //data for nisreq
        struct diag_msg nisreq={0}; //request to send
        int errval;
        nisreq.data = txdata;

        unsigned long t0, chrono;

        if ((len & (128 - 1)) ||
            (start & (128 - 1))) {
            printf("error: misaligned start / length ! \n");
            return -1;
        }

        txdata[0]=SID_FLASH;
        txdata[1]=SIDFL_WB;
        nisreq.len = 134;   //2 (header) + 3 (addr) + 128 (payload) + 1 (extra CRC)

        t0 = diag_os_getms();


        while (remain) {
            uint8_t rxbuf[10];
            unsigned curspeed, tleft;

            chrono = diag_os_getms() - t0;
            if (!chrono) {
                chrono += 1;
            }
            curspeed = 1000 * (len - remain) / chrono;  //avg B/s
            if (!curspeed) {
                curspeed += 1;
            }
            tleft = remain / curspeed;  //s
            if (tleft > 9999) {
                tleft = 9999;
            }

            printf("\rwriting chunk @ 0x%06X (%3u %%, %5u B/s, ~ %4u s remaining)", start, (unsigned) 100 * (len - remain) / len,
                   curspeed, tleft);
            fflush(stdout);

            txdata[2] = start >> 16;
            txdata[3] = start >> 8;
            txdata[4] = start >> 0;
            memcpy(&txdata[5], src, 128);
            txdata[133] = cks_add8(&txdata[2], 131);

            errval = diag_l2_send(global_l2_conn, &nisreq);
            if (errval) {
                printf("l2_send error!\n");
                return -1;
            }

            /* expect exactly 3 bytes, but with generous timeout */
            //rxmsg = diag_l2_request(global_l2_conn, &nisreq, &errval);
            errval = diag_l1_recv(global_l2_conn->diag_link->l2_dl0d, rxbuf, 3, 800);
            if (errval <= 1) {
                printf("\n\tProblem: no response @ %X\n", (unsigned) start);
                (void) diag_l2_ioctl(global_l2_conn, DIAG_IOCTL_IFLUSH, NULL);
                return -1;
            }
            if (errval < 3) {
                printf("\n\tProblem: incomplete response @ %X\n", (unsigned) start);
                (void) diag_l2_ioctl(global_l2_conn, DIAG_IOCTL_IFLUSH, NULL);
                diag_data_dump(stdout, rxbuf, errval);
                printf("\n");
                return -1;
            }

            if (rxbuf[1] != (SID_FLASH + 0x40)) {
                //maybe negative response, if so, get the remaining packet
                printf("\n\tProblem: bad response @ %X: ", (unsigned) start);

                int needed = 1 + rxbuf[0] - errval;
                if (needed > 0) {
                    (void) diag_l1_recv(global_l2_conn->diag_link->l2_dl0d, &rxbuf[errval], needed, 300);
                }
                printf("%s\n", decode_nrc(&rxbuf[1]));
                (void) diag_l2_ioctl(global_l2_conn, DIAG_IOCTL_IFLUSH, NULL);
                return -1;
            }

            remain -= 128;
            start += 128;
            src += 128;

        }   //while len
        printf("\nWrite complete.\n");

        return 0;
    }



//signals:
//    void noppp(QVector<float>);

};

#endif // DMAPROTO_H
