################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GashWindrose.cpp \
../src/MPI.cpp \
../src/MultiCSV.cpp \
../src/MultiCSVOpenMP.cpp \
../src/OpenMPApproach1.cpp \
../src/OpenMPApproach2.cpp \
../src/Parallel.cpp \
../src/ParallelFileReadDemo.cpp \
../src/ParallelFirstDemo.cpp \
../src/ParallelWindRoseReduce.cpp \
../src/ParallelWindrose.cpp \
../src/Serial.cpp \
../src/WindRoseWithExtractedData.cpp \
../src/Windrose.cpp 

OBJS += \
./src/GashWindrose.o \
./src/MPI.o \
./src/MultiCSV.o \
./src/MultiCSVOpenMP.o \
./src/OpenMPApproach1.o \
./src/OpenMPApproach2.o \
./src/Parallel.o \
./src/ParallelFileReadDemo.o \
./src/ParallelFirstDemo.o \
./src/ParallelWindRoseReduce.o \
./src/ParallelWindrose.o \
./src/Serial.o \
./src/WindRoseWithExtractedData.o \
./src/Windrose.o 

CPP_DEPS += \
./src/GashWindrose.d \
./src/MPI.d \
./src/MultiCSV.d \
./src/MultiCSVOpenMP.d \
./src/OpenMPApproach1.d \
./src/OpenMPApproach2.d \
./src/Parallel.d \
./src/ParallelFileReadDemo.d \
./src/ParallelFirstDemo.d \
./src/ParallelWindRoseReduce.d \
./src/ParallelWindrose.d \
./src/Serial.d \
./src/WindRoseWithExtractedData.d \
./src/Windrose.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/openmpi/bin/mpicc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


