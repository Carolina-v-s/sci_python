################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="C:/Users/vieir/Documents/PROJETOS_WORKSPACE/carol/sci_python/microcontroller/src" --include_path="C:/Users/vieir/Documents/PROJETOS_WORKSPACE/carol/sci_python/microcontroller" --include_path="C:/ti/c2000/C2000Ware_5_04_00_00" --include_path="C:/Users/vieir/Documents/PROJETOS_WORKSPACE/carol/sci_python/microcontroller/device" --include_path="C:/ti/c2000/C2000Ware_5_04_00_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=_LAUNCHXL_F28379D --define=DEBUG --define=CPU1 --define=RAM --c99 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/vieir/Documents/PROJETOS_WORKSPACE/carol/sci_python/microcontroller/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-618085873: ../sci.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs1281/ccs/utils/sysconfig_1.21.0/sysconfig_cli.bat" --script "C:/Users/vieir/Documents/PROJETOS_WORKSPACE/carol/sci_python/microcontroller/sci.syscfg" -o "syscfg" -s "C:/ti/c2000/C2000Ware_5_04_00_00/.metadata/sdk.json" -b "/boards/LAUNCHXL_F28379D" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-618085873 ../sci.syscfg
syscfg/board.h: build-618085873
syscfg/board.cmd.genlibs: build-618085873
syscfg/board.opt: build-618085873
syscfg/board.json: build-618085873
syscfg/pinmux.csv: build-618085873
syscfg/adc.dot: build-618085873
syscfg/c2000ware_libraries.cmd.genlibs: build-618085873
syscfg/c2000ware_libraries.opt: build-618085873
syscfg/c2000ware_libraries.c: build-618085873
syscfg/c2000ware_libraries.h: build-618085873
syscfg/clocktree.h: build-618085873
syscfg: build-618085873

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="C:/Users/vieir/Documents/PROJETOS_WORKSPACE/carol/sci_python/microcontroller/src" --include_path="C:/Users/vieir/Documents/PROJETOS_WORKSPACE/carol/sci_python/microcontroller" --include_path="C:/ti/c2000/C2000Ware_5_04_00_00" --include_path="C:/Users/vieir/Documents/PROJETOS_WORKSPACE/carol/sci_python/microcontroller/device" --include_path="C:/ti/c2000/C2000Ware_5_04_00_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=_LAUNCHXL_F28379D --define=DEBUG --define=CPU1 --define=RAM --c99 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/vieir/Documents/PROJETOS_WORKSPACE/carol/sci_python/microcontroller/CPU1_RAM/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


