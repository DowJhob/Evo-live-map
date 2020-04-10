#include "test.h"
#include "ecu_comm.h"
#include <op13.h>
#include <op20.h>

test::test()
{
ECU_Comm *ecucc = new OP13();
ecucc = new OP20();

ecucc->sendDMAcomand(1, 2, 3, new char[9]);
}
