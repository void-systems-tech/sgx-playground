#include <stdlib.h>
#include <string.h>

#include "sgx_playground/datasealing/common.h"


int transform_data(
  const unsigned char* data,
  size_t data_size,
  unsigned char** transformed,
  size_t* transformed_size
) {
  *transformed = NULL;
  *transformed_size = 0;

  if(data == NULL || transformed == NULL || transformed_size == 0) {
    return -1;
  }
  if(data_size == 0) {
    return 0;
  }

  // Output format: "<original> - <reversed>"
  // Size: data_size + 3 (" - ") + data_size + 1 (null terminator)
  size_t sep_len = 3; // " - "
  size_t out_size = data_size + sep_len + data_size + 1;

  unsigned char* out = (unsigned char*) malloc(out_size);
  if(out == NULL) {
    return -1;
  }

  // Original
  memcpy(out, data, data_size);
  // Separator
  out[data_size + 0] = ' ';
  out[data_size + 1] = '-';
  out[data_size + 2] = ' ';
  // Reversed
  for(size_t i = 0; i < data_size; i++) {
    out[data_size + sep_len + i] = data[data_size - 1 - i];
  }
  // Null terminate (convenience, not counted in transformed_size)
  out[out_size - 1] = '\0';

  *transformed      = out;
  *transformed_size = out_size - 1; // exclude null terminator

  return 0;
}


// vim: ts=2 sw=2 et:


