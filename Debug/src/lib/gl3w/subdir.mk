################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/gl3w/gl3w.c 

OBJS += \
./src/lib/gl3w/gl3w.o 

C_DEPS += \
./src/lib/gl3w/gl3w.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/gl3w/%.o: ../src/lib/gl3w/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -std=c11 -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


