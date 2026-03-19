#include <stdio.h>

#include <openenclave/enclave.h>

#include "sgx_playground/datasealing/common.h"

#include "dispatcher.h"
#include "datasealing_t.h"


const char* enclave_name = "EnclaveB";


int enclave_seal_data(
  int sealPolicy,
  const unsigned char* opt_mgs,
  size_t opt_msg_len,
  const unsigned char* data,
  size_t data_size,
  data_t* sealed_data)
{
  fprintf(stderr, "Enclave: %s: seal_data\n", enclave_name);
  return seal_data(
    sealPolicy,
    opt_mgs,
    opt_msg_len,
    data,
    data_size,
    sealed_data
  );
}
int enclave_unseal_data(
    const data_t* sealed_data,
    const int optional_msg_flag,
    data_t* output_data)
{
  fprintf(stderr, "Enclave: %s: unseal_data\n", enclave_name);
  return unseal_data(
    sealed_data,
    optional_msg_flag,
    output_data
  );
}


// vim: ts=2 sw=2 et:


