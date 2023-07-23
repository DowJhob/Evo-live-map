#include "evo7-ecumodel.h"
#include "qendian.h"
#include <QDebug>

evo7_ECUmodel::evo7_ECUmodel()
{

}

bool evo7_ECUmodel::connectDMA()
{
    return ECUmodel::connectDMA(0xF52, 4);
}

