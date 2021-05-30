#ifndef SERIAL_HEADER
#define SERIAL_HEADER

#include <string.h>
#include <stdio.h>
#include "derivative.h"
#include "main.h"

#define SCI_TXREADY 1
#define SCI_TXPROCESSING 0
#define ESCIALLOC -1
#define EOK 1


typedef struct uint8_256buff_t {
  char str[256];
  Byte length;  
  Byte cursor;  
} uint8_256buff_t;

typedef struct {
  SCI0CR1STR SCI_ControlReg1;
  SCI0CR2STR SCI_ControlReg2; 
  Byte SCIn_port; 
} SCI_config_t;

extern uint8_256buff_t tx;
extern uint8_256buff_t rx;
extern Byte SCI_Port_TXRX_Status[2];

Byte set_SCI_config(SCI_config_t);

static char * matrix2D_to_string(const float *matrix, size_t rows, size_t cols);
void send_mesage(Byte SCI_port);

//void send_mesage(struct char256buff msg); 

__interrupt void SCI0_ISR(void);

__interrupt void SCI1_ISR(void);





#endif