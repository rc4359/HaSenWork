//#define _nop()_  _NOP(); 
#include "IO.h"
#include <stdbool.h>
#include "type.h"
#include "data.h"

#define UZ2400D  // define --> zu2400D  , undef --> uz2400
#define RFIC

#undef UZ2400_RX_ENABLE    // define --> enable rx , otherwiae disable
#undef UZ2400_BUFF_RESIZE  // if reduce data buffer of uz2400 fifo
#define SensorLS303  // define --> LS303 , undef --> 3 in 1
#define PERIOD_16MS  // define --> watch dog wake up peroid 16ms , otherwise 250ms
#define  P1_2_SENSOR_CONF  // define --> enable enroll function by press p1.2 button
#undef CK_ENABLE    // 送 正規畫 rf訊號
#define LED_ENABLE




#define SUCCESS 1
#define FAILURE 0
#define WATCH_DOG_DIV   5
typedef union  

{ unsigned int i;
  float f; 
  
} value; 



#define noACK 0 
#define ACK   1 
                            //adr  command  r/w 
#define STATUS_REG_W 0x06   //000   0011    0 
#define STATUS_REG_R 0x07   //000   0011    1 
#define MEASURE_TEMP 0x03   //000   0001    1 
#define MEASURE_HUMI 0x05   //000   0010    1 
#define RESET        0x1e   //000   1111    0 


 
