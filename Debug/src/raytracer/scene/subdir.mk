################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/raytracer/scene/Camera.cpp \
../src/raytracer/scene/Material.cpp \
../src/raytracer/scene/Scene.cpp 

OBJS += \
./src/raytracer/scene/Camera.o \
./src/raytracer/scene/Material.o \
./src/raytracer/scene/Scene.o 

CPP_DEPS += \
./src/raytracer/scene/Camera.d \
./src/raytracer/scene/Material.d \
./src/raytracer/scene/Scene.d 


# Each subdirectory must supply rules for building sources it contributes
src/raytracer/scene/%.o: ../src/raytracer/scene/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


