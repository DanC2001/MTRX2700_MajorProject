#include "main.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

uint8_256buff_t tx;           // Creates tx buffer
uint8_256buff_t rx;           // Creates rx buffer
Byte SCI_Port_TXRX_Status[2]; // Creates status bytes for 2 ports

// Function to make table as a string
static char * matrix2D_to_string(const float *matrix, size_t rows, size_t cols){
  //~~~ Strings in the stack ~~~//
  const char * format         = "%.2f";
  const char * col_seperator  = " ";
  const char * row_seperator  = "\n";
  
  int width = 0;
  int *col_width = NULL;
  size_t r = 0, c = 0, size = 0;
  char *buffer = NULL, *p = NULL;
  char buf[256];
  
  // Calculates maximum width for each column
  col_width = (int *)calloc(cols, sizeof(*col_width));
  
  for(r = 0; r < rows; ++r){
    for(c = 0; c < cols; ++c){
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


    // Define a byte for the SCI port 
    Byte SCI_port;
	
	  // Define a string for table
	  char *s = NULL;
	
  	// Define tilt and pan scan bounds passed of design requirements, includes calibration adjustments
  	unsigned char min_tilt = 60;
  	unsigned char max_tilt = 100;
  	unsigned char min_pan = 50;
  	unsigned char max_pan = 170;
  	
  	// Define increment to increase pan and tilt by
  	unsigned char increment = 10;
  	unsigned char i,j;
  	unsigned char max_count_tilt = (max_tilt-min_tilt)/increment + 1;
  	unsigned char max_count_pan = (max_pan-min_pan)/increment + 1;
  	
  	// Define matrix of distances
  	float distance;
    float distance_matrix[5][13];  /* Set size equal to max count */
    
    // Define central column so can be determined if object is to left or right
    unsigned int central_column = (max_count_pan + 1)/2;
    
    // Keep track of distance to closest object and its position
    float min_distance = 3.0;    /* Currently set within bounds so beeps on first scan */
    int min_column = 1;
    
    
    // Configure Lidar
    DDRH_DDRH0 = 1;     // configure PH0 as output
    PTH_PTH0   = 1;     // Prime the Lidar
  	
  	
  	// Initialise PWM module with a prescaler of 2^3 for both clock A and clock B
  	initialise_PWM(3,3);
  	
  	// Initialise servos
  	initialise_servos();
  	
  	// Initialise the 7-seg LED's
  	initialise_LED();
  	
  	// Initialize the SCI port
    if ((SCI_port = create_SCI_config()) == ESCIALLOC) for(;;) {_FEED_COP();};
  	
  	// Enable interrupts for Lidar & Serial
  	EnableInterrupts;
	
	  

    
	
  	// Scan indefinitely
  	while(1){ 	  
      tx.str[0] = '1';
      tx.str[1] = '2';
      tx.str[2] = '3';
      tx.str[3] = '4';
      tx.str[4] = '5';
      tx.str[5] = '\r';
      tx.str[6] = '\n';
      tx.str[7] = '\0';
      tx.length = 8;
      tx.cursor = 0;
      
      send_mesage(SCI_port);
      
      // Loop through tilt angles
      for(i = 0; i < max_count_tilt; i++){
      
        // Loop through panning angles
        for(j = 0; j < max_count_pan; j++){
          
          // Move servo to calculated position
          move_servos(min_tilt +  i*increment, min_pan + j*increment);
          
          // Pause for half a second
          ms_delay(20);
                    
          // Take measurement
          //distance = getRange();
          
          distance = 0.01;
          
          // Save measuremnt into corresponding location in the distance matrix
          distance_matrix[i][j] = distance;
          
          single_beep(1); 
          
        }
        
      }
      // Make and send table to serial
      
      s = matrix2D_to_string((const float *)distance_matrix, ARRAY_SIZE(distance_matrix), ARRAY_SIZE(distance_matrix[0]));
      strcpy(tx.str, s);
      tx.length = strlen(s);
      tx.cursor = 0;
      send_mesage(SCI_port);
      
      // Display reccomended direction of travel to the user
      LED_display(min_column, central_column);      
      
      // Feed the dog after each scan to avoid timeout reset
      _FEED_COP();
      
  	}
    

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

/** The minimuim duty cycle allowed by the servos */
//#define MIN_DUTY_CYCLE 2700
/** The maximium duty cycle allowed by the servos */
//#define MAX_DUTY_CYCLE 6300
/** The period of the PWM signal */
//#define PERIOD 60000

/*
// Basic function to return a distance
float scan_distanc(void){
  
  float distance = 1.5;
  return distance;
}

// Function to move servos
unsigned char move_servos(float tilt, float pan){
	
	unsigned int tilt_duty_cycle , pan_duty_cycle;
	
	if( tilt>180.0 || tilt<0.0 || pan>180.0 || pan<0.0 ) return 0;
	tilt_duty_cycle = MIN_DUTY_CYCLE + (20 * tilt);
	pan_duty_cycle = MIN_DUTY_CYCLE + (20 * pan);
		
//	assert(elevationDutyCycle <= MAX_DUTY_CYCLE && elevationDutyCycle >= MIN_DUTY_CYCLE && azimuthDutyCycle <= MAX_DUTY_CYCLE && azimuthDutyCycle >= MIN_DUTY_CYCLE);
		
	PWMDTY45 = tilt_duty_cycle;
	PWMDTY67 = pan_duty_cycle;
	return 1;
}

// Function to initialise servos
void initialise_servos(void){

  // Configure Port P data direction register for output
	DDRP = 0xFF;
	
	// Set each concatenated period channel to the calculated period value
	PWMPER45 = PERIOD;
	PWMPER67 = PERIOD;

  // Set each duty cycle to be minimum plus 90 degrees so that servos initially start from central location
	PWMDTY45 = MIN_DUTY_CYCLE + (20 * 90);
	PWMDTY67 = MIN_DUTY_CYCLE + (20 * 90);
}

// Function to initialise PWM settings
void initialise_PWM(int clock_A , int clock_B){
  
  // Set the prescalers of clock A and B
	PWMPRCLK = clock_A + (clock_B << 4);
  
  // Concatenate channels 4 and 5 to create a 16 bit channel
  PWMCTL_CON45 = 0x01;
  
  // Concatenate channels 6 and 7 to create a 16 bit channel
  PWMCTL_CON67 = 0x01;
  
  // Configure pulse to be left aligned
	PWMCAE = 0x00;
	
	// Set polarity to 1
	PWMPOL = 0xFF;
	
	// Enable PWM channels
	PWME = 0xF0;
}

// Function causes time delay of 1 millisecond * input parameter
void ms_delay(unsigned int time_ms){

    // Define counter variables to count back from
    // Volatile helps ensure this is not optimised out by compiler
    volatile int i; 
    volatile int j;
    
    // Define length of delay required for 1 ms
    volatile int ms_delay = 4000;
    
    // Waste time for number of inputted milliseconds
    for(i = 0; i < time_ms ; i++) {
      
      for(j = 0 ; j <ms_delay ; j++){
      }
    }
}

*/