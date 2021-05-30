#include "serial.h"

// function to initialise the timer
Byte set_SCI_config(SCI_config_t port_parameters) {
  //extern Byte SCI_port;
  
  if (port_parameters.SCIn_port == 0) {
    SCI0BD = (word) 156; /* 9600 */
    SCI0CR1 = port_parameters.SCI_ControlReg1.Byte;  
    SCI0CR2 = port_parameters.SCI_ControlReg2.Byte;
    if (!SCI0SR1_TDRE) return ESCIALLOC;
    SCI_Port_TXRX_Status[port_parameters.SCIn_port] = SCI_TXREADY;
    return 0;  
  } else {
    SCI1BD = (word) 156; /* 9600 */
    SCI1CR1 = port_parameters.SCI_ControlReg1.Byte;  
    SCI1CR2 = port_parameters.SCI_ControlReg2.Byte;
    if (!SCI1SR1_TDRE) return ESCIALLOC;
    SCI_Port_TXRX_Status[port_parameters.SCIn_port] = SCI_TXREADY;
    return 1; 
  }
}

void send_mesage(Byte SCI_port) {
  if (tx.cursor < tx.length) {
    SCI_Port_TXRX_Status[SCI_port] = SCI_TXPROCESSING;
    if (SCI_port == 0) {
      SCI0DRL = tx.str[tx.cursor];
      tx.cursor++;
      SCI0CR2_SCTIE = 1;
    } else {
      SCI1DRL = tx.str[tx.cursor];
      tx.cursor++;
      SCI1CR2_SCTIE = 1;  
    }
  }     
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void SCI0_ISR(void){
    if (SCI0SR1_RDRF) {
      rx.str[rx.length++] = SCI0DRL;
      if (rx.length >= 256) {
        rx.length = 0;
      }
    } else if (SCI0SR1_TDRE) {
      if (tx.cursor < tx.length) {
        SCI0DRL = tx.str[tx.cursor];
        tx.cursor++;
      } else {
        SCI0CR2_SCTIE = 0;
        //SCI1DRL = 0;
        tx.cursor = 0;
        SCI_Port_TXRX_Status[0] = SCI_TXREADY;
      }
    } 
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void SCI1_ISR(void){
    if (SCI1SR1_RDRF) {
      rx.str[rx.length++] = SCI1DRL;
      if (rx.length >= 256) {
        rx.length = 0;
      }
    } else if (SCI1SR1_TDRE) {
      if (tx.cursor < tx.length) {
        SCI1DRL = tx.str[tx.cursor];
        tx.cursor++;
      } else {
        SCI1CR2_SCTIE = 0;
        //SCI1DRL = 0;
        tx.cursor = 0;
        SCI_Port_TXRX_Status[1] = SCI_TXREADY;
      }
    } 
}




