#ifndef HG_SGX_PLAYGROUND_DATASEALING_COMMON_H
#define HG_SGX_PLAYGROUND_DATASEALING_COMMON_H

#include <openenclave/bits/types.h>


#define POLICY_UNIQUE   OE_SEAL_POLICY_UNIQUE
#define POLICY_PRODUCT  OE_SEAL_POLICY_PRODUCT

#define GET_POLICY_NAME(policy) ((policy == POLICY_UNIQUE) ? "POLICY_UNIQUE" : "POLICY_PRODUCT")


#define MAX_OPTIONAL_MESSAGE_SIZE 128


#define ERROR_UNSEAL_FAIL        1
#define ERROR_OUT_OF_MEMORY      2
#define ERROR_UNSEALED_DATA_FAIL 6
#define ERROR_SEAL_FAIL   7

#define GET_ERROR_NAME(error) (\
  (error == ERROR_UNSEAL_FAIL) ? "ERROR_UNSEAL_FAIL" : \
  (error == ERROR_OUT_OF_MEMORY) ? "ERROR_OUT_OF_MEMORY" : \
  (error == ERROR_UNSEALED_DATA_FAIL) ? "ERROR_UNSEALED_DATA_FAIL" : \
  (error == ERROR_SEAL_FAIL) ? "ERROR_SEAL_FAIL" : \
  "UNKNOWN_ERROR"\
)


int transform_data(
  const unsigned char* data,
  size_t data_size,
  unsigned char** transformed,
  size_t* transformed_size
);


#endif // HG_SGX_PLAYGROUND_DATASEALING_COMMON_H


// vim: ts=2 sw=2 et:


