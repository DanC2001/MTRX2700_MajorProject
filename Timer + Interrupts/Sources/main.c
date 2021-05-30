#include "main.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

uint8_256buff_t tx;           // Creates tx buffer
uint8_256buff_t rx;           // Creates rx buffer
Byte SCI_Port_TXRX_Status[2]; // Creates status bytes for 2 ports


// Function to make table as a string
static char * matrix2D_to_string(const float *matrix, size_t rows, size_t cols){
  //~~~ Strings in the stack ~~~//
  const char * format         = "%.2f";
  const char * col_seperator  = "  ";
  const char * row_seperator  = "\n";
  
  int width = 0;
  int *col_width = NULL;
  size_t r = 0, c = 0, size = 0;
  char *buffer = NULL, *p = NULL;
  
  // Calculates maximum width for each column
  col_width = (int *)calloc(cols, sizeof(*col_width));
  
  for(r = 0; r < rows; ++r){
    for(c = 0; c < cols; ++c){
      char buf[256];
      width = sprintf(buf, format, matrix[r * cols + c]);
      if(width > col_width[c]){
        col_width[c] = width;
      }
    }
  }
  
  // Calculate the total buffer size taking...
  // ... Each value into account
  for(c = 0; c < cols; ++c){
    size += col_width[c] * rows;
  }
  
  // ... Column seperators into account
  size += (cols - 1) * strlen(col_seperator);
  
  // ... Row seperators into account
  size += (rows - 1) * strlen(row_seperator);
  
  // ... Null terminator into account
  size++;
  
  // Make the string
  buffer = (char*)malloc(size);
  p = buffer;
  for(r = 0; r < rows; ++r){
    if(r){
      strcpy(p,row_seperator);
      p += strlen(row_seperator); 
    }
    for(c = 0; c < cols; ++c){
      if(c){
        strcpy(p,col_seperator);
        p += strlen(col_seperator); 
      }
      width = sprintf(p,format,matrix[r * cols +c]);
      p += width;
      if(width < col_width[c]){
        width = col_width[c] - width;
        memset(p,' ', width);
        p += width; 
      }
    }
  }
  *p = '\0';
  
  //cleanup
  free(col_width);
  
  return buffer;
}


void main(void) {
  Byte SCI_port;
  
  float M[3][2]={{1.2,3.4},{3.14,2.718},{100.999,0.000005}};

  char *s = NULL;
  
  s = matrix2D_to_string((const float *)M, ARRAY_SIZE(M), ARRAY_SIZE(M[0]));

  strcpy(tx.str, s);
  
  // Initialize the SCI port
  if ((SCI_port = create_SCI_config()) == ESCIALLOC) for(;;) {_FEED_COP();}; 
  
  tx.length = (Byte) rx.str[0] - 48;
  tx.cursor = 0;
  
  EnableInterrupts;
  
  send_mesage(SCI_port);
  
  free(s);

  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */

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