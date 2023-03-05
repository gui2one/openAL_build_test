cmake_minimum_required(VERSION 3.2)

project(openAL_build_test)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

include_directories(
    src/
    vendor/openal/include
    vendor/wave/src
)


# OPENAL
add_subdirectory(vendor/openal)
target_link_libraries(${PROJECT_NAME} OpenAL ${OPENAL_LIBRARIES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${CMAKE_BINARY_DIR}/vendor/openal/${CMAKE_BUILD_TYPE}/OpenAL32.dll"      # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)  

# WAVE
set(wave_enable_tests OFF)
add_subdirectory(vendor/wave)
target_link_libraries(${PROJECT_NAME} wave ${WAVE_LIBRARIES})
