#include "mtos.h"

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
