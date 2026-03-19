add_library(
  datasealing_common_trusted
  OBJECT
  common.c
)
target_link_libraries(
  datasealing_common_trusted
  PUBLIC
    project_includes
    openenclave::oeenclave
)

add_library(
  datasealing_common_untrusted
  OBJECT
  common.c
)
target_link_libraries(
  datasealing_common_untrusted
  PUBLIC
    project_includes
    openenclave::oehost
)


# vim: ts=2 sw=2 et:


