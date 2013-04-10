################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Account.cpp \
../src/Client.cpp \
../src/DataLayer.cpp \
../src/PesentationLayer.cpp \
../src/SessionController.cpp \
../src/Staff.cpp \
../src/main.cpp 

OBJS += \
./src/Account.o \
./src/Client.o \
./src/DataLayer.o \
./src/PesentationLayer.o \
./src/SessionController.o \
./src/Staff.o \
./src/main.o 

CPP_DEPS += \
./src/Account.d \
./src/Client.d \
./src/DataLayer.d \
./src/PesentationLayer.d \
./src/SessionController.d \
./src/Staff.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


