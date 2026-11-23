cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME test_frameworks_f1)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C CXX ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    User_code/app/app.cpp
    User_code/bsp/uart/uart.cpp
    User_code/bsp/gpio/gpio.cpp
    User_code/bsp/can/can.cpp
    User_code/bsp/pwm/pwm.cpp
    User_code/bsp/i2c/i2c.cpp
    User_code/bsp/adc/adc.cpp
    User_code/bsp/error/error.cpp
    User_code/bsp/log/log.cpp
    User_code/bsp/device/device.cpp
    User_code/module/servo/servo.cpp
    User_code/module/motor/motor.cpp
    User_code/module/mpu6050/mpu6050.cpp
    User_code/module/sensor/sensor.cpp
    User_code/module/laser/laser.cpp
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${CMAKE_SOURCE_DIR}/app
    ${CMAKE_SOURCE_DIR}/bsp
    ${CMAKE_SOURCE_DIR}/module
)

# 把 User_code 加入头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/User_code
)


# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti>
    $<$<CONFIG:Debug>:-Og>
    $<$<CONFIG:Release>:-Os>
    -Wall
    -Wextra
    -mcpu=cortex-m3
    -mthumb
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    # 链接脚本已在 cmake/gcc-arm-none-eabi.cmake 里指定，这里不要再写 -T，否则重复
    -mcpu=cortex-m3
    -mthumb
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generate hex / bin"
)
