# Build the enclave.
add_executable(
  enclave_datasealing_b
  main.c
)
target_link_libraries(
  enclave_datasealing_b
  PRIVATE
    datasealing
    edl_datasealing_trusted
)

# Sign the enclave.
sign_enclave(
  enclave_datasealing_b
  KEY_TARGET
    datasealing_product_b_key
)


# vim: ts=2 sw=2 et:


