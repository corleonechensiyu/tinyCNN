cmake_minimum_required(VERSION 3.9)

project(testBackend)


add_definitions(-std=c++11)
add_definitions("-Wall")
add_definitions(-DUNICODE -D_UNICODE)
find_package(OpenMP REQUIRED)


if(OPENMP_FOUND)
    message("openMP found")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/include/
    ${CMAKE_CURRENT_LIST_DIR}/src/
    ${CMAKE_CURRENT_LIST_DIR}/src/layer/
    ${CMAKE_CURRENT_LIST_DIR}/3rd/flatbuffers/include
)



set(SRC 
    src/Mat.cpp 
    src/Backend.cpp 
    src/BufferAllocator.cpp 
    src/memoryutils.cpp 
    src/layerRegister.cpp

    src/layer/convolution.cpp 
    src/layer/innerproduct.cpp 
    src/layer/pooling.cpp 
    src/layer/relu.cpp 
    src/layer/softmax.cpp

    test/testBackend.cpp
)

add_executable(testBackend ${SRC})
#target_link_libraries(testBackend PUBLIC TCNN)