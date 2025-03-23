# Define sources
target_sources(${PROJECT_NAME} PRIVATE
        # Add project source files here
        # Example:
        # ${SOURCE_DIR}/Folder/Example.cpp
        # ${SOURCE_DIR}/Folder/Example.h

        # ---
        "${SOURCE_DIR}/Game.cpp"
        "${SOURCE_DIR}/Game.h"
)

add_custom_command(TARGET ${PROJECT_NAME} 
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/DATA"
        "${CMAKE_BINARY_DIR}/DATA"
        )