cmake_minimum_required(VERSION 3.2)

project(openAL_build_test)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

include_directories(

    vendor/openal/include
)

add_subdirectory(vendor/openal)
target_link_libraries(${PROJECT_NAME} openal ${OPENAL_LIBRARIES})