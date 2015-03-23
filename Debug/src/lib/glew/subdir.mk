################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/glew/glew.c 

OBJS += \
./src/lib/glew/glew.o 

C_DEPS += \
./src/lib/glew/glew.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/glew/%.o: ../src/lib/glew/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -std=c11 -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


