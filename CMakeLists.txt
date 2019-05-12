project(minimal_kernel)
cmake_minimum_required(VERSION 2.8)


enable_language(C ASM)

set(CMAKE_SYSTEM_PROCESSOR arm)


set(CMAKE_ASM_COMPILER "arm-none-eabi-as")
set(CMAKE_C_COMPILER   "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_LINKER       "arm-none-eabi-ld")
set(LINKER_FILE        "${CMAKE_SOURCE_DIR}/etc/main.ld")


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=arm926ej-s")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_FILE}" )
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)



add_executable(kernel.elf src/startup.s src/main.c)
#target_link_libraries(kernel.elf gcc)


add_custom_command(
	TARGET  kernel.elf POST_BUILD
	COMMAND arm-none-eabi-objcopy ./kernel.elf -O binary ./kernel.img
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	COMMENT "Convert the ELF output file to a binary image"
)
