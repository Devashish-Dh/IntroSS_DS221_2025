CMAKE_MINIMUM_REQUIRED ( VERSION 3.18)

PROJECT(ISSassg1
        VERSION 1.0
        DESCRIPTION "running the codes" 
        LANGUAGES CXX        
)
set(CMAKE_BUILD_TYPE DEBUG)
SET ( CMAKE_CXX_STANDARD 17 )

ADD_EXECUTABLE ( ${CMAKE_PROJECT_NAME} # remember to include the .cpp and .cu files here 
	main.cpp
	user_code.h
	file_reader.h
	file_writer.h
	

)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -pg -g -fno-omit-frame-pointer -funroll-loops -fopt-info-vec" )
message(STATUS "CXX Flags: ${CMAKE_CXX_FLAGS}" )

TARGET_COMPILE_OPTIONS( ${CMAKE_PROJECT_NAME}
    PRIVATE $<$<COMPILE_LANGUAGE:CXX>: >
    )



