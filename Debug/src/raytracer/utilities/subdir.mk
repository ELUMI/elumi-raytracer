################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/raytracer/utilities/Ray.cpp \
../src/raytracer/utilities/Triangle.cpp 

OBJS += \
./src/raytracer/utilities/Ray.o \
./src/raytracer/utilities/Triangle.o 

CPP_DEPS += \
./src/raytracer/utilities/Ray.d \
./src/raytracer/utilities/Triangle.d 


# Each subdirectory must supply rules for building sources it contributes
src/raytracer/utilities/%.o: ../src/raytracer/utilities/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


