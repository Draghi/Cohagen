################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/glew2/glew.c 

OBJS += \
./src/lib/glew2/glew.o 

C_DEPS += \
./src/lib/glew2/glew.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/glew2/%.o: ../src/lib/glew2/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -std=c11 -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


