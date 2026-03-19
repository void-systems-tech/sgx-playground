add_library(
  datasealing
  OBJECT
  ${CMAKE_CURRENT_SOURCE_DIR}/dispatcher.c
)
target_include_directories(
  datasealing
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(
  datasealing
  PUBLIC
    datasealing_common_trusted
    edl_datasealing_trusted
  INTERFACE
    $<TARGET_OBJECTS:datasealing_common_trusted>
    $<TARGET_OBJECTS:openenclave::oeseal_gcmaes>
    openenclave::oecryptombedtls
)

add_custom_target(
  datasealing_product_a_key
  COMMAND
    openssl genrsa -out ${CMAKE_CURRENT_BINARY_DIR}/datasealing_product_a_private.pem -3 3072
  BYPRODUCTS
    ${CMAKE_CURRENT_BINARY_DIR}/datasealing_product_a_private.pem
)
set_target_properties(
  datasealing_product_a_key
  PROPERTIES
    SIGNING_KEY ${CMAKE_CURRENT_BINARY_DIR}/datasealing_product_a_private.pem
)

add_custom_target(
  datasealing_product_b_key
  COMMAND
    openssl genrsa -out ${CMAKE_CURRENT_BINARY_DIR}/datasealing_product_b_private.pem -3 3072
  BYPRODUCTS
    ${CMAKE_CURRENT_BINARY_DIR}/datasealing_product_b_private.pem
)
set_target_properties(
  datasealing_product_b_key
  PROPERTIES
    SIGNING_KEY ${CMAKE_CURRENT_BINARY_DIR}/datasealing_product_b_private.pem
)


add_subdirectory(a_v1)
add_subdirectory(a_v2)
add_subdirectory(b)


# vim: ts=2 sw=2 et:


