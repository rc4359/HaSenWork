#include "msp430x22x4.h"
#include "../Header/Io.h"


#define LDOOut    33   // 3.0v*10
#define PIRHighWinVol  20   // high windows   2.0V
#define PIRLowWinVol   11   // low windows    1.1V
#define OFFDelaySec    10*4  // 10sec  (250ms*4*10)=10sec