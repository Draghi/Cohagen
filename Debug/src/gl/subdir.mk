################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gl/FBO.c \
../src/gl/MatrixManager.c \
../src/gl/RBO.c \
../src/gl/Textures.c \
../src/gl/VAO.c \
../src/gl/VBO.c 

OBJS += \
./src/gl/FBO.o \
./src/gl/MatrixManager.o \
./src/gl/RBO.o \
./src/gl/Textures.o \
./src/gl/VAO.o \
./src/gl/VBO.o 

C_DEPS += \
./src/gl/FBO.d \
./src/gl/MatrixManager.d \
./src/gl/RBO.d \
./src/gl/Textures.d \
./src/gl/VAO.d \
./src/gl/VBO.d 


# Each subdirectory must supply rules for building sources it contributes
src/gl/%.o: ../src/gl/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -std=c11 -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


