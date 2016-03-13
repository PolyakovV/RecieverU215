// Target : CC430F5137
#include <cc430x513x.h>
#include  <intrinsics.h>
#include "HAL/RF1A.h"
#include "HAL/cc430x613x_PMM.h"
#include "HAL/HAL_FLASH.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "RF_Connection.h"
#include  "usart.h"
#include  "Valik_os.h"


/****************************************************************************/
/*  Function name: delay_us                                                 */
/*  	Parameters                                                          */
/*          Input   :  p	                                            */
/*          Output  :  No	                                            */
/*	Action: Simple delay                                                */
/****************************************************************************/
void delay_1_us(volatile unsigned long p){}

void Delay_50us(void){
unsigned long l=43;//l=56;
  while (l){ l--;delay_1_us(1);}
}


void Delay_ms(volatile unsigned long p){
unsigned long l=p*20;
while (l){ l--;Delay_50us();}
}

int str_len (const char *str)
{
    return (*str) ? str_len(++str) + 1 : 0;
}


int process_attach(char *name, int prio, void *function)
{
        int i = 0;
        int ret = -1;
        while(i < MAX_PROCESSES) {
                if(strlen(name) > MAX_NAME_LEN) {
                        return ret;
                }
                if(processlist[i].attached != 1) {
                        processlist[i].pid = i;
                        strcpy(processlist[i].name, name);
                        processlist[i].prio = prio;
                        processlist[i].function =(int *) function;
                        processlist[i].attached = 1;
                        ret = 0;
                        break;
                }
                i++;
        }
        return ret;
 
}
 
int process_detach(int pid)
{
        processlist[pid].attached = 0;
        return 0;
}

int process_detach_by_name (char *name){

  for(int i = 0; i < MAX_PROCESSES; i++) {
                       
                        if(processlist[i].attached == 1) {
                          if(  strcmp(name,processlist[i].name)==0) {
                            processlist[i].pid = 0;
                            processlist[i].prio =0;
                            processlist[i].attached = 0;
                            processlist[i].function = 0;
                            strcpy(processlist[i].name,"");

                          }
                        }
  }
return 0;
}


int scheduler()
{
        int i = 0;
       void (*pgm_start_address)(void);
        while(1) {
                for(i = 0; i < MAX_PROCESSES; i++) {
                        if(processlist[i].attached == 1) {
                        pgm_start_address = (void(*)(void))processlist[i].function;
                        pgm_start_address();
                       // Delay_ms(200);Delay_ms(200);
                        }
                }
        }
        return 0;
}

/////////////////////// Mediator ////////////////////////////
int process_subscribe (char *subscr_event, char *subscr_name, int *function){
 
  int i = 0;
  int ret = -1;
                  if(str_len(subscr_name) > MAX_NAME_LEN) {
                                    return ret;
                            }
        while(i < MAX_SUBSCRIBERS) {
                 if (subscribers_list[i].attached != 1){
                        subscribers_list[i].pid = i;
                        subscribers_list[i].function =(int *) function;
                        strcpy(subscribers_list[i].subscr_name,subscr_name);
                        strcpy(subscribers_list[i].subscr_event,subscr_event);
                        subscribers_list[i].attached = 1;
                        
                        ret = 0;
                        break;}
                i++;
        }
        return ret;
}

///////////////////////////////////////////////////////////////////////
//
//
int process_unsubscribe_by_name (char *subscr_name){
int i = 0;
  
while(i < MAX_SUBSCRIBERS){
    if (subscribers_list[i].attached == 1 ){
    if (strcmp(subscr_name,subscribers_list[i].subscr_name)==0){
                        subscribers_list[i].pid = 0;
                        subscribers_list[i].function = 0;
                        strcpy(subscribers_list[i].subscr_name,"");
                        strcpy(subscribers_list[i].subscr_event,"");
                        subscribers_list[i].attached = 0;
      }
    }
    i++;
    }
return 0;
}
///////////////////////////////////////////////////////////////////////
//
//
int process_unsubscribe_by_event (char *subscr_event){
int i = 0;
  
while(i < MAX_SUBSCRIBERS){
    if (subscribers_list[i].attached == 1 ){
    if (strcmp(subscr_event,subscribers_list[i].subscr_event)==0){
                         subscribers_list[i].pid = 0;
                        subscribers_list[i].function = 0;
                        strcpy(subscribers_list[i].subscr_name,"");
                        strcpy(subscribers_list[i].subscr_event,"");
                        subscribers_list[i].attached = 0;
      }
    }
    i++;
    }
return 0;
}
//////////////////////////////////////////////////////////////////////
//
//
//
int process_trigger (char *subscr_event){
  int i = 0;
  int (*pgm_start_address)(void);
  while(i < MAX_SUBSCRIBERS){
    if (subscribers_list[i].attached == 1 ){
    if (strcmp(subscr_event,subscribers_list[i].subscr_event)==0){
      pgm_start_address = (int(*)(void))subscribers_list[i].function; // get address of function
      //USART_Send_ROM_String("\n\r");
      //USART_Send_ROM_String(subscribers_list[i].subscr_name);
      pgm_start_address(); // go to this address
      }
    }
    i++;
    }
  return 0;
}















