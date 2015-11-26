# Log sub-directory cmake config

get_sources(local_src local_headers local_includes)
set (SOURCES "${SOURCES} ${local_src}" PARENT_SCOPE)
set (HEADERS ${HEADERS} ${local_headers} PARENT_SCOPE)
set (INCLUDES ${INCLUDES} ${local_includes} PARENT_SCOPE)

