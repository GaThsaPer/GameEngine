# Define sources
target_sources(${PROJECT_NAME} PRIVATE
        # Add project source files here
        # Example:
        # ${SOURCE_DIR}/Folder/Example.cpp
        # ${SOURCE_DIR}/Folder/Example.h

        # ---
        "${SOURCE_DIR}/Game.cpp"
        "${SOURCE_DIR}/Game.h"

        #Add animation class
        "${SOURCE_DIR}/Visuals/AnimatedSprite.cpp"
        "${SOURCE_DIR}/Visuals/AnimatedSprite.h"

        #Add Input
        "${SOURCE_DIR}/Input/Input.cpp"
        "${SOURCE_DIR}/Input/Input.h"
        "${SOURCE_DIR}/Input/InputCodes.h"

        #Add Physic
        "${SOURCE_DIR}/Physics/Body.h"
        "${SOURCE_DIR}/Physics/Body.cpp"
        "${SOURCE_DIR}/Physics/Shapes.h"
        "${SOURCE_DIR}/Physics/Shapes.cpp"
)

add_custom_command(TARGET ${PROJECT_NAME} 
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/DATA"
        "${CMAKE_BINARY_DIR}/DATA"
        )
