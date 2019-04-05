# Examples

# Create "hv/" headers layout in the build dir
if (EXISTS ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME_LOWER_WP}.h)
	file(COPY ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME_LOWER_WP}.h
			DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/${HV_LIB_PREFIX})
endif()
file(COPY "${PROJECT_SOURCE_DIR}/src/"
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/${HV_LIB_PREFIX}/${PROJECT_NAME_LOWER_WP}
		FILES_MATCHING
		REGEX "${PROJECT_SOURCE_DIR}/${PROJECT_NAME_LOWER_WP}.h" EXCLUDE
		PATTERN "*.h"
		PATTERN "*.hpp")
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

add_subdirectory(1-simple-param)
add_subdirectory(2-cci)
add_subdirectory(3-config-yaml)
