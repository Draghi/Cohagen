################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/math/Mat3.c \
../src/math/Mat4.c \
../src/math/Quat.c \
../src/math/Vec2.c \
../src/math/Vec3.c \
../src/math/Vec4.c 

OBJS += \
./src/math/Mat3.o \
./src/math/Mat4.o \
./src/math/Quat.o \
./src/math/Vec2.o \
./src/math/Vec3.o \
./src/math/Vec4.o 

C_DEPS += \
./src/math/Mat3.d \
./src/math/Mat4.d \
./src/math/Quat.d \
./src/math/Vec2.d \
./src/math/Vec3.d \
./src/math/Vec4.d 


# Each subdirectory must supply rules for building sources it contributes
src/math/%.o: ../src/math/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -std=c11 -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


