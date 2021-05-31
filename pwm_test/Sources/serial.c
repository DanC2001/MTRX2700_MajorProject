#include "serial.h"

// function to initialise the serial ports
Byte set_SCI_config(SCI_config_t port_parameters) {
  // Input is a SCI config structure
  
  // If SCI0
  if (port_parameters.SCIn_port == 0) {
    SCI0BD = (word) 156; /* Baud 9600 */
    SCI0CR1 = port_parameters.SCI_ControlReg1.Byte;  // Setting CR1
    SCI0CR2 = port_parameters.SCI_ControlReg2.Byte;  // Setting CR2
    if (!SCI0SR1_TDRE) return ESCIALLOC;  // If successful return true signal (-1)
    SCI_Port_TXRX_Status[port_parameters.SCIn_port] = SCI_TXREADY;
    return 0;  
  } else { // If SCI1
    SCI1BD = (word) 156; /* Baud 9600 */
    SCI1CR1 = port_parameters.SCI_ControlReg1.Byte;  // Setting CR1
    SCI1CR2 = port_parameters.SCI_ControlReg2.Byte;  // Setting CR2
    if (!SCI1SR1_TDRE) return ESCIALLOC;  // If successful return true signal (-1)
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
      SCI0CR2_SCTIE = 1;           // push it through
    } else {             // If SCI1
      SCI1DRL = tx.str[tx.cursor]; // load caharacter
      tx.cursor++;                 // increment cursor
      SCI1CR2_SCTIE = 1;           // push it through
    }
  }     
}

/*// Function to make table as a string
void tabulateMeasures(float **matrix, int iMax, int jMax, char *string){
  int row, col;
  
  for(row = 0; row < iMaz; row++){
    
    for(col = 0; col < jMax; col++){
      
    }
  }
  
  
  
} */


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Interupt Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// SCI0 Interrupt Function
#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void SCI0_ISR(void){  
    if (SCI0SR1_RDRF) { // If SCI0 in reciever mode
      rx.str[rx.length++] = SCI0DRL; // Store value and increment cursor
      if (rx.length >= 256) { // If larger than buffer size loop back to 0
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

// SCI0 Interrupt Function
#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void SCI1_ISR(void){
    if (SCI1SR1_RDRF) { // If SCI1 in reciever mode
      rx.str[rx.length++] = SCI1DRL;  // Store value and increment cursor
      if (rx.length >= 256) { // If larger than buffer size loop back to 0
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




