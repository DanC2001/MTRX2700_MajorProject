#include "main.h"


uint8_256buff_t tx;           // Creates tx buffer
uint8_256buff_t rx;           // Creates rx buffer
Byte SCI_Port_TXRX_Status[2]; // Creates status bytes for 2 ports


void main(void) {
  /* Setup (Pre Interrupts) */
  
  Byte SCI_port;
  
  if ((SCI_port = create_SCI_config()) == ESCIALLOC) for(;;) {_FEED_COP();}; 
  
	EnableInterrupts;     // enable the interrupts - calls asm("CLI");
	  
  tx.str[0] = '1';
  tx.str[1] = '2';
  tx.str[2] = '3';
  tx.str[3] = '4';
  tx.str[4] = '5';
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

// Function to make the default settings for serial ports
Byte create_SCI_config(void) {
  SCI_config_t SCInCR_settings; // Make a new SCI port setrting structure
   
  // SCInCR1
  SCInCR_settings.SCI_ControlReg1.Bits.PT = 0;
  SCInCR_settings.SCI_ControlReg1.Bits.PE = 0;
  SCInCR_settings.SCI_ControlReg1.Bits.ILT = 0;
  SCInCR_settings.SCI_ControlReg1.Bits.WAKE = 1;   
  SCInCR_settings.SCI_ControlReg1.Bits.M = 1;
  SCInCR_settings.SCI_ControlReg1.Bits.RSRC = 0;
  SCInCR_settings.SCI_ControlReg1.Bits.SCISWAI = 0;
  SCInCR_settings.SCI_ControlReg1.Bits.LOOPS = 0;
  
  // SCInCR2
  SCInCR_settings.SCI_ControlReg2.Bits.SBK = 0;
  SCInCR_settings.SCI_ControlReg2.Bits.RWU = 0; 
  SCInCR_settings.SCI_ControlReg2.Bits.RE = 1;
  SCInCR_settings.SCI_ControlReg2.Bits.TE = 1;
  SCInCR_settings.SCI_ControlReg2.Bits.ILIE = 0;
  SCInCR_settings.SCI_ControlReg2.Bits.RIE = 1;
  SCInCR_settings.SCI_ControlReg2.Bits.TCIE = 0;
  SCInCR_settings.SCI_ControlReg2.Bits.SCTIE = 0; //transmit interrupt
  
  // Port Selection (Change for different port, 0 = SCI0, 1 = SCI1) 
  SCInCR_settings.SCIn_port = 1;

  // Call config fuction that will complete the configuration and return success value
  return set_SCI_config(SCInCR_settings);   
}

