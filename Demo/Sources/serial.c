#include "serial.h"

#include <assert.h> /* Assert header for error catching */

// Function to make the default settings for serial ports
Byte create_SCI_config(byte SCI_port) {
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
  SCInCR_settings.SCIn_port = SCI_port;

  // Call config fuction that will complete the configuration and return success value
  return set_SCI_config(SCInCR_settings);   
}

// function to initialise the serial ports
Byte set_SCI_config(SCI_config_t port_parameters) {
  // Input is a SCI config structure
  
  // If SCI0
  if (port_parameters.SCIn_port == 0) {
    SCI0BD = (word) 156; /* Baud 9600 */
    SCI0CR1 = port_parameters.SCI_ControlReg1.Byte;  // Setting CR1
    SCI0CR2 = port_parameters.SCI_ControlReg2.Byte;  // Setting CR2
    if (!SCI0SR1_TDRE) return ESCIALLOC;  // If unsuccessful return signal (-1)
    SCI_Port_TXRX_Status[port_parameters.SCIn_port] = SCI_TXREADY;
    return 0;  
  } else { // If SCI1
    SCI1BD = (word) 156; /* Baud 9600 */
    SCI1CR1 = port_parameters.SCI_ControlReg1.Byte;  // Setting CR1
    SCI1CR2 = port_parameters.SCI_ControlReg2.Byte;  // Setting CR2
    if (!SCI1SR1_TDRE) return ESCIALLOC;  // If unsuccessful return signal (-1)
    SCI_Port_TXRX_Status[port_parameters.SCIn_port] = SCI_TXREADY;
    return 1; 
  }
}

// Function to send a message to the port input using the tx buffer
void send_mesage(Byte SCI_port) {
  if (tx.cursor < tx.length) {  // if still message left
    SCI_Port_TXRX_Status[SCI_port] = SCI_TXPROCESSING;
    if (SCI_port == 0) { // If SCI0
      SCI0DRL = tx.str[tx.cursor]; // load caharacter
      tx.cursor++;                 // increment cursor
      SCI0CR2_SCTIE = 1;           // enable interupt
    } else {             // If SCI1
      SCI1DRL = tx.str[tx.cursor]; // load caharacter
      tx.cursor++;                 // increment cursor
      SCI1CR2_SCTIE = 1;           // enable interupt
    }
  }     
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Interupt Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// SCI0 Interrupt Function
#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void SCI0_ISR(void){  
    if (SCI0SR1_RDRF) { // If SCI0 in reciever mode
      rx.str[rx.length++] = SCI0DRL; // Store value and increment cursor
      if (rx.length >= 512) { // If larger than buffer size loop back to 0
        rx.length = 0;        // THIS WILL OVERWRITE THE BEGINING OF THE BUFFER
      }
    } else if (SCI0SR1_TDRE) {  // If SCI0 in transmit mode
      if (tx.cursor < tx.length) { // If cursor is less then end of buffer
        SCI0DRL = tx.str[tx.cursor]; // Load character
        tx.cursor++;                 // Increment cursor
      } else {
        SCI0CR2_SCTIE = 0;    // Turn off interupt
        //SCI1DRL = 0;
        tx.cursor = 0;        // Reset the cursor
        SCI_Port_TXRX_Status[0] = SCI_TXREADY;
      }
    } 
}

// SCI1 Interrupt Function
#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void SCI1_ISR(void){
    if (SCI1SR1_RDRF) { // If SCI1 in reciever mode
      rx.str[rx.length++] = SCI1DRL;  // Store value and increment cursor
      if (rx.length >= 512) { // If larger than buffer size loop back to 0
        rx.length = 0;        // THIS WILL OVERWRITE THE BEGINING OF THE BUFFER
      }
    } else if (SCI1SR1_TDRE) {  // if SCI1 in transmit mode
      if (tx.cursor < tx.length) {  // If cursor is less then end of buffer
        SCI1DRL = tx.str[tx.cursor];  // Load character
        tx.cursor++;                  // Increment cursor
      } else {
        SCI1CR2_SCTIE = 0;   // Turn off interupt 
        //SCI1DRL = 0;
        tx.cursor = 0;       // Reset the cursor
        SCI_Port_TXRX_Status[1] = SCI_TXREADY;
      }
    } 
}




