#ifndef SERIAL_HEADER
#define SERIAL_HEADER

// This header file contains the functions relating to the Serial module which is used
// to output strings to the user using the serical communications interface
// The function create_SCI_config is called from main
// The function set_SCI_config is called from create_SCI_config
// The function send_message is called from main
// The interrupt routine SCI0_ISR is called when interrupt vector 0x14 is made
// The interrupt routine SCI1_ISR is called when interrupt vector 0x15 is made
// Last edited by Daniel Cook on 2/06/2021
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"

// Definitions
#define SCI_TXREADY 1       // when able to be called for transmission
#define SCI_TXPROCESSING 0  // when processing transmission
#define ESCIALLOC -1        // Error for unable to allocate SCI port


// Structure for buffers
typedef struct uint8_512buff_t {
  char str[512];            // 512 character buffer
  unsigned int length;      // Length identifier (for identifying working length of buffer)
  unsigned int cursor;      // Cursor (tracking working character)
} uint8_512buff_t;

// Structure for SCI configuration settings
typedef struct {
  SCI0CR1STR SCI_ControlReg1; // CR1 (Using MCS12 Definitions of register from mc9s12dg256.h)
  SCI0CR2STR SCI_ControlReg2; // CR2 (Using MCS12 Definitions of register from mc9s12dg256.h)
  Byte SCIn_port;             // Byte to track what port number this setting is for 
} SCI_config_t;

extern uint8_512buff_t tx;    // Gets scope of tx from main
extern uint8_512buff_t rx;    // Gets scope of rx from main
extern Byte SCI_Port_TXRX_Status[2];  // Gets scope of status bytes from main

// Function to make the config settings of serial ports Outputs the serial port number if 
// successful, -1 (255) if unsuccessful (this is from the output of set_SCI_config)
Byte create_SCI_config(byte SCI_port);

// Function to set the configuration made by create_SCI_config. Outputs the serial port number
// if successful, -1 (255) if unsuccessful
Byte set_SCI_config(SCI_config_t);

// Function to start the process of sending a message to the port input using the tx buffer
void send_mesage(Byte SCI_port);

// SCI0 Interrupt Function
__interrupt void SCI0_ISR(void);

// SCI1 Interrupt Function
__interrupt void SCI1_ISR(void);





#endif