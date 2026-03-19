# Build the host binary: `host_hello`.
add_executable(
  host_hello
  hello.c
)
target_link_libraries(
  host_hello
  PRIVATE
    edl_hello_untrusted
)
add_executable(
  host_datasealing
  datasealing.c
)
target_link_libraries(
  host_datasealing
  PRIVATE
    datasealing_common_untrusted
    edl_datasealing_untrusted
)


# vim: ts=2 sw=2 et:


