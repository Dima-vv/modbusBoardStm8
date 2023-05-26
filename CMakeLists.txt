cmake_minimum_required(VERSION 3.2)

set(CMAKE_C_OUTPUT_EXTENSION ".rel")
set(CMAKE_C_COMPILER sdcc)
set(CMAKE_SYSTEM_NAME Generic) # No linux target etc

# Prevent default configuration
set(CMAKE_C_FLAGS_INIT "")
set(CMAKE_EXE_LINKER_FLAGS_INIT "")

project(STM8Modbus C)
SET(CMAKE_C_FLAGS "-mstm8 --model-medium --std-c99 --opt-code-size")
set(SPL_PATH STM8S-SDCC-SPL/)
set(SPL_SRC_PATH ${SPL_PATH}/src)
include_directories(${SPL_PATH}/inc ${SPL_PATH}/conf General PetitModbus AFTER)
include_directories(. BEFORE)
# Configure which STM8S we are using
add_definitions(-DSTM8S003)

add_executable(main
    main.c
    init.c
    ${SPL_SRC_PATH}/stm8s_gpio_Init.c
    ${SPL_SRC_PATH}/stm8s_gpio_WriteLow.c
    ${SPL_SRC_PATH}/stm8s_gpio_WriteHigh.c
    ${SPL_SRC_PATH}/stm8s_gpio_ReadInputPin.c
    #${SPL_SRC_PATH}/stm8s_gpio_Write.c
    ${SPL_SRC_PATH}/stm8s_tim2_Cmd.c
    ${SPL_SRC_PATH}/stm8s_tim2_TimeBaseInit.c
    ${SPL_SRC_PATH}/stm8s_tim2_DeInit.c
    ${SPL_SRC_PATH}/stm8s_tim2_ITConfig.c
#    ${SPL_SRC_PATH}/stm8s_tim2_ClearFlag.c
#    ${SPL_SRC_PATH}/stm8s_tim2_SetCounter.c
    ${SPL_SRC_PATH}/stm8s_tim2_ClearITPendingBit.c
    ${SPL_SRC_PATH}/stm8s_clk_private.c
    ${SPL_SRC_PATH}/stm8s_clk_GetClockFreq.c
    ${SPL_SRC_PATH}/stm8s_clk_ClockSwitchConfig.c
    ${SPL_SRC_PATH}/stm8s_uart1_GetFlagStatus.c
    ${SPL_SRC_PATH}/stm8s_uart1_SendData8.c
#    ${SPL_SRC_PATH}/stm8s_uart1_SendBreak.c
    ${SPL_SRC_PATH}/stm8s_uart1_ReceiveData8.c
    ${SPL_SRC_PATH}/stm8s_uart1_DeInit.c
    ${SPL_SRC_PATH}/stm8s_uart1_Init.c
    ${SPL_SRC_PATH}/stm8s_uart1_ITConfig.c
    ${SPL_SRC_PATH}/stm8s_clk_PeripheralClockConfig.c
    ${SPL_SRC_PATH}/stm8s_gpio_ExternalPullUpConfig.c
    ${SPL_SRC_PATH}/stm8s_clk_DeInit.c
    ${SPL_SRC_PATH}/stm8s_clk_SYSCLKConfig.c
    ${SPL_SRC_PATH}/stm8s_uart1_ClearFlag.c
    ${SPL_SRC_PATH}/stm8s_uart1_ClearITPendingBit.c
    ${SPL_SRC_PATH}/stm8s_uart1_Cmd.c
    ${SPL_SRC_PATH}/stm8s_flash_DeInit.c
    ${SPL_SRC_PATH}/stm8s_flash_ReadByte.c
    ${SPL_SRC_PATH}/stm8s_flash_ProgramByte.c
    ${SPL_SRC_PATH}/stm8s_flash_Unlock.c
    ${SPL_SRC_PATH}/stm8s_flash_Lock.c
    PetitModbus/PetitModbus.c
    PetitModbus/PetitModbusPort.c
    PetitModbus/PetitRegConstant.c
)



# Flash targets
add_custom_target(flash COMMAND stm8flash -c stlinkv2 -p stm8s003?3 -w main.ihx)
