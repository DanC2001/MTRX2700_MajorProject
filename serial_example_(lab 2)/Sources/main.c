#include "main.h"


uint8_256buff_t tx;
uint8_256buff_t rx;
Byte SCI_Port_TXRX_Status[2];


void main(void) {
  /* Setup (Pre Interrupts) */
  
  Byte SCI_port;
  
  if ((SCI_port = create_SCI_config()) == ESCIALLOC) for(;;) {_FEED_COP();}; 
  
	EnableInterrupts;     // enable the interrupts - calls asm("CLI");
	
	while(rx.length == 0);
  
  tx.str[0] = 'h';
  tx.str[1] = 'e';
  tx.str[2] = 'l';
  tx.str[3] = 'l';
  tx.str[4] = 'o';
  tx.str[5] = '\r';
  tx.str[6] = '\n';
  tx.str[7] = '\0';
  tx.length = (Byte) rx.str[0] - 48;
  tx.cursor = 0;

  send_mesage(SCI_port);
  
  while(rx.length == 1);
  tx.length = (Byte) rx.str[1] - 48;
  tx.cursor = 0;
  
  send_mesage(SCI_port);
  
  while(rx.length == 2);	

	/* Loop (Post Interrupts) */
	
//	while(1);


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* never leave main */
}


Byte create_SCI_config(void) {
  SCI_config_t SCInCR_settings;
   
  
  SCInCR_settings.SCI_ControlReg1.Bits.PT = 0;
  SCInCR_settings.SCI_ControlReg1.Bits.PE = 0;
  SCInCR_settings.SCI_ControlReg1.Bits.ILT = 0;
  SCInCR_settings.SCI_ControlReg1.Bits.WAKE = 1;   
  SCInCR_settings.SCI_ControlReg1.Bits.M = 1;
  SCInCR_settings.SCI_ControlReg1.Bits.RSRC = 0;
  SCInCR_settings.SCI_ControlReg1.Bits.SCISWAI = 0;
  SCInCR_settings.SCI_ControlReg1.Bits.LOOPS = 0;
  
  SCInCR_settings.SCI_ControlReg2.Bits.SBK = 0;
  SCInCR_settings.SCI_ControlReg2.Bits.RWU = 0; 
  SCInCR_settings.SCI_ControlReg2.Bits.RE = 1;
  SCInCR_settings.SCI_ControlReg2.Bits.TE = 1;
  SCInCR_settings.SCI_ControlReg2.Bits.ILIE = 0;
  SCInCR_settings.SCI_ControlReg2.Bits.RIE = 1;
  SCInCR_settings.SCI_ControlReg2.Bits.TCIE = 0;
  SCInCR_settings.SCI_ControlReg2.Bits.SCTIE = 0; //transmit interrupt
   
  SCInCR_settings.SCIn_port = 1;

  return set_SCI_config(SCInCR_settings);   
}

