

#define PIRAnlog (1<<1)   // PIR Anlog input at A1(P2.1 ) 
#define A1_PIR (1<<12);


#define PIRIODIR  P2DIR
#define PIRIOIN   P2IN
#define PIRIOPIN  (1<<5)

//----------------------------------------
//---------- SPI IO define --------------------
#define SpiDin   0x20
#define SpiDout  0x10
#define SpiClk   0x01
#define SpiInt   0x40
#define SpiCs    0x80
#define SpiNC    0x02
#define Uz24SpDIR   P3DIR
#define Uz24SpOUT   P3OUT
#define Uz24SpIN    P3IN


// ----------  Enroll key -------------------
#define EnrollPort P1DIR

#define EnrollKey  (1<<2)
#define isKeyRess  !(P1IN&EnrollKey)


// LED IO //
#define LED_1_PIN (1<<7)
#define LED_2_PIN (1<<6)

#define LED_1_SET_OUT  P1DIR|=LED_1_PIN
#define LED_2_SET_OUT  P1DIR|=LED_2_PIN

#define LED_1_ON       P1OUT|=LED_1_PIN
#define LED_2_ON       P1OUT|=LED_2_PIN
#define LED_1_OFF       P1OUT&=~LED_1_PIN
#define LED_2_OFF       P1OUT&=~LED_2_PIN

#define RF_GOT_ACK_LED_ON  LED_1_ON
#define RF_GOT_ACK_LED_OFF  LED_1_OFF

#define LOW_POER_LED_ON   LED_2_ON
#define LOW_POER_LED_OFF  LED_2_OFF


