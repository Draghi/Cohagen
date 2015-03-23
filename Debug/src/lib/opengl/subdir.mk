################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/opengl/gl_compat_3_0.c 

OBJS += \
./src/lib/opengl/gl_compat_3_0.o 

C_DEPS += \
./src/lib/opengl/gl_compat_3_0.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/opengl/%.o: ../src/lib/opengl/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -std=c11 -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


