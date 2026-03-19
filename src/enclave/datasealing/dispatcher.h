#ifndef HG_SGX_PLAYGROUND_SRC_ENCLAVE_DATASEALING_DISPATCHER_H
#define HG_SGX_PLAYGROUND_SRC_ENCLAVE_DATASEALING_DISPATCHER_H


#include "datasealing_args.h"


int seal_data(
  int seal_policy,
  const unsigned char* opt_mgs,
  size_t opt_msg_len,
  const unsigned char* data,
  size_t data_size,
  data_t* sealed_data
);

int unseal_data(
  const data_t* sealed_data,
  const int opt_msg_flag,
  data_t* output_data
);


#endif // HG_SGX_PLAYGROUND_SRC_ENCLAVE_DATASEALING_DISPATCHER_H


// vim: ts=2 sw=2 et:


