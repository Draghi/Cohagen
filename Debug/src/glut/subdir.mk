################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/glut/Display.c \
../src/glut/Keyboard.c \
../src/glut/Mouse.c 

OBJS += \
./src/glut/Display.o \
./src/glut/Keyboard.o \
./src/glut/Mouse.o 

C_DEPS += \
./src/glut/Display.d \
./src/glut/Keyboard.d \
./src/glut/Mouse.d 


# Each subdirectory must supply rules for building sources it contributes
src/glut/%.o: ../src/glut/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -std=c11 -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


