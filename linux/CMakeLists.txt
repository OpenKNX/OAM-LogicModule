cmake_minimum_required(VERSION 2.7)
project(knx-logikmodul)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(LIBRARIES_FROM_REFERENCES "")
add_executable(knx-logikmodul 
	../../libraries/knx/src/knx/address_table_object.cpp 
	../../libraries/knx/src/knx/apdu.cpp 
	../../libraries/knx/src/knx/application_layer.cpp 
	../../libraries/knx/src/knx/application_program_object.cpp 
	../../libraries/knx/src/knx/association_table_object.cpp 
	../../libraries/knx/src/knx/bau.cpp 
	../../libraries/knx/src/knx/bau07B0.cpp 
	../../libraries/knx/src/knx/bau57B0.cpp 
	../../libraries/knx/src/knx/bau_systemB.cpp 
	../../libraries/knx/src/knx/bits.cpp 
	../../libraries/knx/src/knx/cemi_frame.cpp 
	../../libraries/knx/src/knx/data_link_layer.cpp 
	../../libraries/knx/src/knx/device_object.cpp 
	../../libraries/knx/src/knx/group_object.cpp 
	../../libraries/knx/src/knx/group_object_table_object.cpp 
	../../libraries/knx/src/knx/interface_object.cpp 
	../../libraries/knx/src/knx/ip_data_link_layer.cpp 
	../../libraries/knx/src/knx/ip_parameter_object.cpp 
	../../libraries/knx/src/knx/memory.cpp 
	../../libraries/knx/src/knx/network_layer.cpp 
	../../libraries/knx/src/knx/npdu.cpp 
	../../libraries/knx/src/knx/table_object.cpp 
	../../libraries/knx/src/knx/tpdu.cpp 
	../../libraries/knx/src/knx/tpuart_data_link_layer.cpp 
	../../libraries/knx/src/knx/transport_layer.cpp 
	../../libraries/knx/src/knx/platform.cpp 
	main.cpp 
	../logikmodul.cpp 
	../../libraries/knx/src/linux_platform.cpp 
	../../libraries/knx/src/knx_facade.cpp 
	../../libraries/knx/src/knx/dptconvert.cpp
	../../libraries/knx/src/knx/knx_value.cpp
	../../libraries/knx/src/knx/dpt.cpp)
target_link_libraries(knx-logikmodul  "${LIBRARIES_FROM_REFERENCES}")
include_directories(../../libraries/knx/src)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wno-unknown-pragmas -Wno-switch -g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wno-unknown-pragmas -Wno-switch -g -O0")
if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Debug")
endif()
