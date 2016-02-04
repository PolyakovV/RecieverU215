/****************************************************************************/
/* Reciver                                                                  */
/****************************************************************************/
// Target : CC430F5137
#include <cc430x513x.h>
#include  <intrinsics.h>
#include "HAL/RF1A.h"
#include "HAL/cc430x613x_PMM.h"
//#include "RF_Connection.h"
#include  "usart.h"
//////////////////////////////////////////////////////////////////////////

void Init_RF(void); 
void delay(volatile unsigned long p);
void delay_1us(volatile unsigned long p);
void delay_ms(volatile unsigned long p);
void Delay50us(void);
//////////////////////////////////////////////////////////////////////////

#define CPU_FREQ   26000000

//Definitions
#define LED_On			P1OUT |= 0x01;		P1DIR |= 0x01;
#define LED_Off		        P1OUT &= (~0x01);	P1DIR |= 0x01;
#define LED_Togg		P1OUT ^= 0x01;		P1DIR |= 0x01;
#define LED_Chk		        (P1IN & 0x01)

#define LED1_On			P1OUT |= BIT2;		P1DIR |= BIT2;
#define LED1_Off	        P1OUT &= (~BIT2);	P1DIR |= BIT2;

#define LED2_On			P1OUT |= BIT3;		P1DIR |= BIT3;
#define LED2_Off	        P1OUT &= (~BIT3);	P1DIR |= BIT3;

#define LED3_On			P1OUT |= BIT4;		P1DIR |= BIT4;
#define LED3_Off	        P1OUT &= (~BIT4);	P1DIR |= BIT4;

#define Pin_RX                  (P1IN & BIT1)
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
  
  // Set up the button as interruptible 
  //P1DIR &= ~BIT1;
  //P1REN |= BIT1;
  //P1IES &= BIT1;
  //P1IFG = 0;
  //P1OUT |= BIT1;
  //P1IE  |= BIT1; 

  // Set up LEDs 
  P1OUT &= ~BIT0;
  P1DIR |= BIT0;
  
 
  
  // Set up BUTTON 
  P1OUT |= BIT1;  //Pullup
  P1DIR &= BIT1;  //Input
  
  
  P1MAP0|=BIT0;
  P1DIR |=BIT0;
  P1SEL |=BIT0;
}
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*                       INTERRUPTS                                         */
/*                                                                          */
/*                                                                          */
/****************************************************************************/


/**********************************************************************************/
/*      PORT1, interrupt service routine.                                         */
/**********************************************************************************/
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
  switch(__even_in_range(P1IV, 4))
  {
    case  0: break;
    case  2: break;                         // P1.0 IFG
    case  4:
      P1IE = 0;                             // 
        
      break;                                // P1.1 IFG
    case  6: break;                         // P1.2 IFG
    case  8: break;                         // P1.3 IFG
    case 10: break;                         // P1.4 IFG
    case 12: break;                         // P1.5 IFG
    case 14: break;                         // P1.6 IFG
    case 16: break;                         // P1.7 IFG 
      
  }
}

/****************************************************************************/
/*     LOCAL VARIABLES                                                      */
/****************************************************************************/

char rssi_char;
unsigned char sRX[5];
unsigned char data_right;
unsigned char j,k,l,m;
int lCount,hCount,allCount;
char vRSSI_1=0,vRSSI_2=0;
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*                       MAIN                                               */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
int main(void){
    
  //1. Stop errant interrupts until set up
 _BIC_SR(GIE); // Disable interrupts during initialization process
 WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
 
 ports_init();
 Init_RF();
 _BIS_SR(GIE); // Global Interrupt enabled. Do this at the END of the initialization!!!!!!!!
 USART_Init();

 // USART_Send_ROM_String(testString);

//-------------------CLOCK------------------------------------------------
// ACLK = 32kHz, MCLK = SMCLK = 16MHz

P5SEL |= BIT0 + BIT1; // Select XT1
UCSCTL6 |= XCAP_3; // Internal load cap

__bis_SR_register(SCG0);                // Disable the FLL control loop
UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
UCSCTL1 = DCORSEL_7;                // Select DCO range 16MHz operation
UCSCTL2 = FLLD_1 + 487;              // Set DCO Multiplier for 8MHz
                                                              // (N + 1) * FLLRef = Fdco
                                                              // (249 + 1) * 32768 = 8MHz
                                                              // (365 + 1) * 32768 = 12MHz
                                                              // (487 + 1) * 32768 = 16MHz
                                                              // Set FLL Div = fDCOCLK/2

__bic_SR_register(SCG0); // Enable the FLL control loop

// Worst-case settling time for the DCO when the DCO range bits have been
// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
// UG for optimization.
// 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle

__delay_cycles(375000);

 USART_Init();
 UCA0MCTL = UCBRS_0 + UCBRF_3 + UCOS16; 
 UCA0BR0 = 103;                              // 1MHz 9600 (see User's Guide)
 UCA0BR1 = 0; 
 delay_ms(200);
 
 //USART_Send_ROM_Menu_Begin();
 //USART_Send_Data(vRSSI_2);

 
  
  while(1){ 
  //asm("NOP");
 // if (Pin_RX == 0) {LED1_On;} else {LED1_Off;}
 
// Delay50us();
//LED1_On;
//  Delay50us();
//LED1_Off;


  /* rssi_char = ReadSingleReg(RSSI);
  USART_Send_Data(rssi_char);
    
    if (rssi_char <= 25) LED1_On;
    if (rssi_char <= 15) LED2_On;
    if (rssi_char <= 5) LED3_On;
    delay(200); 
    
    LED1_Off;
    LED2_Off;
    LED3_Off;
    asm("NOP");
  */
  //////////////////////
   // USART_Send_Data(rssi_char);
   // rssi_char = ReadSingleReg(RSSI);
   // if (rssi_char <= 255) LED3_On;
   // if (rssi_char <= 100) LED2_On;
   // if (rssi_char <= 5) LED1_On;
    //delay(200); 
    
    
    
    
	if (Pin_RX >= 1){		
			lCount = 0;
		 	while (Pin_RX) {									  
                                        lCount++;
                                        Delay50us();
                                       }
			if ((lCount >= 6) && (lCount <=10)){					//0.4msµ?°?????	
				sRX[0] = 0;
				sRX[1] = 0;
				sRX[2] = 0;
				sRX[3] = 0;
				sRX[4] = 0;
				data_right = 1;	
				//Pin_test = 1;				
				vRSSI_1 = ReadSingleReg(RSSI);
  				
				for (j=0;j<36;j++){
					lCount = 0;
					hCount = 0;
                                        
				   	while (!Pin_RX) {									  //µ?µ???????
						lCount++;
						if (lCount > 20) {
                                                                  lCount = 100;
                                                                  break;
						                  }
						Delay50us();
                                                
					}
                                        
					while (Pin_RX) {									  //??µ???????
						hCount++;
						if (hCount > 20) {
                                                                  hCount = 100;
                                                                  break;
                                                                  }
						Delay50us();
					}
					allCount = lCount + hCount;
					if ((allCount >= 4) && (allCount <= 45)){			  //12,28
						if (lCount > hCount){
							sRX[j / 8] = sRX[j / 8] | (1 << (j % 8));
							
						}else{
							
						}
					} else{
						data_right = 0;
						break;
					      }
				}
				//Pin_test = 0;
				Delay50us();
				if (data_right == 1){
					vRSSI_2 = ReadSingleReg(RSSI);
					k = 0;							
					for (j=0; j<4; j++){
						l = sRX[j] & 0x0f;
						k ^= l; 
						m = sRX[j] >> 4;
						k ^= m;				
					} 
					if (k == sRX[4]){ LED1_On;
						          USART_Send_Data(0x00);
                                                          for (j=0;j<5;j++){
                                                                            USART_Send_Data(sRX[j]);
                                                                            }
                                                                            USART_Send_Data(vRSSI_1);
                                                                            USART_Send_Data(vRSSI_2);
					                                    USART_Send_Data(0x00);
                                                                            LED1_Off;
					                } 
														 
				} 			
			}
		}
//////////////////////////// 
   // LED1_Off;
   // LED2_Off;
   // LED3_Off;
  }
  
}

/*****************************************************************************************/
/*                                                                                       */
/*  	 HEADERs LIBRARIES FUNCTIONS                                                     */
/*  	                                                                                 */
/*****************************************************************************************/


/**********************************************************************************/
/*  Function name: Init_RF               	                                  */
/*  	Parameters                                                                */
/*          Input   :  No       			                          */
/*          Output  :  No	                                                  */
/*	Action: Initialize RF radio core.                               	  */
/**********************************************************************************/
void Init_RF(void){
  
  // Increase PMMCOREV level to 2 in order to avoid low voltage error 
  // when the RF core is enabled
  SetVCore(2);
  ResetRadioCore();     
 // WriteBurstReg(IOCFG2, (unsigned char*)RF1A_REGISTER_CONFIG, CONF_REG_SIZE);

WriteSingleReg(		IOCFG0	,	0x0C	);
WriteSingleReg(		IOCFG2 	,	0x33	);
WriteSingleReg(		MDMCFG0	,	0xF8	);
WriteSingleReg(		MDMCFG1	,	0x22	);
WriteSingleReg(		FREQ0	,	0x89	);
WriteSingleReg(		FREQ1	,	0x1D	);
WriteSingleReg(		FREQ2	,	0x0C	);
WriteSingleReg(		MDMCFG2	,	0x30	);
WriteSingleReg(		MDMCFG3	,	0xE4	);
WriteSingleReg(		MDMCFG4	,	0xf6	);//0x56
WriteSingleReg(		MCSM0	,	0x18	);
WriteSingleReg(		PKTCTRL0,	0x35	);
WriteSingleReg(		AGCCTRL0,	0x90	);
WriteSingleReg(		AGCCTRL2,	0x07	);
WriteSingleReg(		FREND0	,	0x11	);
WriteSingleReg(		FREND1	,	0x66	);
WriteSingleReg(		FSCAL0	,	0x02	);
WriteSingleReg(		AGCCTRL1,	0x40	);
WriteSingleReg(		0x7E	,	0x00	);
WriteSingleReg(		0xCC	,	0x00	);
WriteSingleReg(		IOCFG2	,	0x00	);
WriteSingleReg(		IOCFG2	,	0x00	);
WriteSingleReg(		IOCFG0 	,	0x0D	);

WriteSingleReg(		AGCCTRL2,	0x88	);
WriteSingleReg(         AGCCTRL1,       0x30    );
WriteSingleReg(		FSCAL3,         0xE9	);
WriteSingleReg(		FSCAL2,         0x2A	);
WriteSingleReg(		FSCAL1,         0x00	);
WriteSingleReg(		FSCAL0,         0x1F	);

WriteSingleReg( MCSM1, 0x3C);

  WritePATable();
  ReceiveOn();  
  //Wait for RX status to be reached
  while((Strobe(RF_SNOP) & 0x70) != 0x10);
  
}

/****************************************************************************/
/*  Function name: delay                                                    */
/*  	Parameters                                                          */
/*          Input   :  p	                                            */
/*          Output  :  No	                                            */
/*	Action: Simple delay                                                */
/****************************************************************************/
void delay(volatile unsigned long p){
	while(p){p--;}
}

/****************************************************************************/
/*  Function name: delay_us                                                 */
/*  	Parameters                                                          */
/*          Input   :  p	                                            */
/*          Output  :  No	                                            */
/*	Action: Simple delay                                                */
/****************************************************************************/
void delay_1us(volatile unsigned long p){}

void delay_ms(volatile unsigned long p){
unsigned long l=p*1150;
while (l){ l--;delay_1us(1);}
}

void Delay50us(void){
unsigned long l=43;//l=56;
  while (l){ l--;delay_1us(1);}
}