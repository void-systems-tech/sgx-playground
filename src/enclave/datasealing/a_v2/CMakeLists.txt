# Build the enclave.
add_executable(
  enclave_datasealing_a_v2
  main.c
)
target_link_libraries(
  enclave_datasealing_a_v2
  PRIVATE
    datasealing
    edl_datasealing_trusted
)

# Sign the enclave.
sign_enclave(
  enclave_datasealing_a_v2
  KEY_TARGET
    datasealing_product_a_key
)


# vim: ts=2 sw=2 et:


