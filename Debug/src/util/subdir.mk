################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/util/DynamicArray.c \
../src/util/DynamicFloatArray.c \
../src/util/DynamicIntArray.c \
../src/util/Log.c \
../src/util/OGLUtil.c \
../src/util/ObjLoader.c \
../src/util/Shader.c \
../src/util/ShaderLoader.c \
../src/util/Stack.c 

OBJS += \
./src/util/DynamicArray.o \
./src/util/DynamicFloatArray.o \
./src/util/DynamicIntArray.o \
./src/util/Log.o \
./src/util/OGLUtil.o \
./src/util/ObjLoader.o \
./src/util/Shader.o \
./src/util/ShaderLoader.o \
./src/util/Stack.o 

C_DEPS += \
./src/util/DynamicArray.d \
./src/util/DynamicFloatArray.d \
./src/util/DynamicIntArray.d \
./src/util/Log.d \
./src/util/OGLUtil.d \
./src/util/ObjLoader.d \
./src/util/Shader.d \
./src/util/ShaderLoader.d \
./src/util/Stack.d 


# Each subdirectory must supply rules for building sources it contributes
src/util/%.o: ../src/util/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -std=c11 -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


