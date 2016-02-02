/****************************************************************************/
/* My testing program                                                       */
/****************************************************************************/
// Target : CC430F5137
#include <cc430x513x.h>
#include  <intrinsics.h>
#include  "usart.h"
#include "RF_Connection.h"

//Definitions
#define LED_On			P1OUT |= 0x01;		P1DIR |= 0x01;
#define LED_Off		        P1OUT &= (~0x01);	P1DIR |= 0x01;
#define LED_Togg		P1OUT ^= 0x01;		P1DIR |= 0x01;
#define LED_Chk		        (P1IN & 0x01)

#define LED1_On			P1OUT |= BIT2;		P1DIR |= BIT2;
#define LED1_Off	        P1OUT &= (~BIT2);	P1DIR |= BIT2;

/****************************************************************************/
/*  Function name: ports_init                                               */
/*  	Parameters                                                          */
/*          Input   :  No	                                            */
/*          Output  :  No	                                            */
/*	Action: Initialize all Port's directions and states                 */
/****************************************************************************/
void ports_init(void)
{
  P1OUT = 0x00;
  P1DIR = 0x00; 
  P1REN = 0x00;
  P2OUT = 0x00;
  P2DIR = 0x00;
  P2REN = 0x00;
  P3OUT = 0x00;
  P3DIR = 0x00;
  P3REN = 0x00;
  
}


/****************************************************************************/
/*  Function name: init_devices                                             */
/*  	Parameters                                                          */
/*          Input   :  No	                                            */
/*          Output  :  No	                                            */
/*	Action: Initialize all peripherals                                  */
/****************************************************************************/
void init_devices(void)
{
 //1. Stop errant interrupts until set up
 _BIC_SR(GIE); // Disable interrupts during initialization process
  USART_Init(); 
 ports_init();

 _BIS_SR(GIE); // Global Interrupt enabled. Do this at the END of the initialization!!!!!!!!
 //all peripherals are now initialized
}


int main(void){
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  init_devices();
  USART_Send_ROM_Menu_Begin();
  while(1){ 
    asm("NOP");
    RF_Connection_Test();
    asm("NOP");
  }
  
}

