# Example

get_filename_component(EXAMPLE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" EXAMPLE_NAME ${EXAMPLE_NAME})

file(GLOB ${EXAMPLE_NAME}_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${EXAMPLE_NAME} ${${EXAMPLE_NAME}_FILES})

set(${EXAMPLE_NAME}-LIBRARIES ${CMAKE_PROJECT_NAME_LOWER}
		SystemC::systemc
		cciapi)

target_link_libraries(${EXAMPLE_NAME} ${${EXAMPLE_NAME}-LIBRARIES})
