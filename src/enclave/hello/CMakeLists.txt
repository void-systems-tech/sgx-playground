# Build the enclave.
add_executable(
  enclave_hello
  main.c
)
target_link_libraries(
  enclave_hello
  PRIVATE
    edl_hello_trusted
)

# Sign the enclave.
sign_enclave(enclave_hello)


# vim: ts=2 sw=2 et:


