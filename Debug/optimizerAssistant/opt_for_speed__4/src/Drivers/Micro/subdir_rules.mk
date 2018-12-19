################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
src/Drivers/Micro/%.obj: ../src/Drivers/Micro/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.4.LTS/bin/cl430" -vmspx --abi=eabi --code_model=small --data_model=small -O3 --opt_for_speed=3 --use_hw_mpy=F5 --include_path="C:/Users/mats/Desktop/IOLabRemoteSensor/src/Include" --include_path="C:/Users/mats/Desktop/IOLabRemoteSensor/src/Drivers/Utilities" --include_path="C:/Users/mats/Desktop/IOLabRemoteSensor/src/Drivers/Micro" --include_path="C:/Users/mats/Desktop/IOLabRemoteSensor/src/Drivers/Devices" --include_path="C:/Users/mats/Desktop/IOLabRemoteSensor/src/Components" --include_path="C:/Users/mats/Desktop/IOLabRemoteSensor/src/Application" --include_path="C:/ti/ccsv8/ccs_base/msp430/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-msp430_18.1.4.LTS/include" -g --relaxed_ansi --define=__MSP430F5329__ --display_error_number --diag_warning=225 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=nofloat --preproc_with_compile --preproc_dependency="src/Drivers/Micro/$(basename $(<F)).d_raw" --obj_directory="src/Drivers/Micro" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


