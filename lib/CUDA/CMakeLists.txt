##############################################################
#   ____                                                     #
#  | __ )  ___ _ __  ___ _   _ _ __   ___ _ __ _ __   ___    #
#  |  _ \ / _ \ '_ \/ __| | | | '_ \ / _ \ '__| '_ \ / __|   #
#  | |_) |  __/ | | \__ \ |_| | |_) |  __/ |  | |_) | (__    #
#  |____/ \___|_| |_|___/\__,_| .__/ \___|_|  | .__/ \___|   #
#                             |_|             |_|            #
##############################################################
#                                                            #
#  BenLib, 2021                                              #
#  Created: 16, March, 2021                                  #
#  Modified: 17, March, 2021                                 #
#  file: CMakeLists.txt                                      #
#  CMake                                                     #
#  Source:  https://developer.nvidia.com/blog/building-cuda-applications-cmake/                                                   #
#           https://docs.nvidia.com/cuda/cuda-c-best-practices-guide/index.html
#           https://github.com/robertmaynard/code-samples/blob/master/posts/cmake/CMakeLists.txt
#           https://cuda-tutorial.readthedocs.io/en/latest/tutorials/tutorial01/
#           https://stackoverflow.com/questions/33442355/cmake-file-for-c-cuda-project
#           https://stackoverflow.com/questions/58861467/how-to-compile-c-as-cuda-using-cmake
#           https://gist.github.com/ashwin/6547060
#           https://stackoverflow.com/questions/61056517/build-cuda-kernel-with-meson
#           https://mesonbuild.com/Tutorial.html
#           https://cmake.org/cmake/help/git-stage/prop_tgt/CUDA_ARCHITECTURES.html
#           https://docs.nvidia.com/cuda/cuda-compiler-driver-nvcc/index.html#gpu-feature-list
#           https://github.com/onqtam/awesome-cmake
#           https://hothardware.com/news/cuda-on-intel-gpus-zluda
#           https://stackoverflow.com/questions/18273760/cmake-how-to-add-cuda-to-existing-project
#           https://developer.download.nvidia.com/video/gputechconf/gtc/2019/presentation/s9444-build-systems-exploring-modern-cmake-cuda-v2.pdf
#  OS: ALL                                                   #
#  CPU: ALL                                                  #
#                                                            #
##############################################################

cmake_minimum_required(VERSION 3.9)
project(my_cuda LANGUAGES CUDA CXX C)


#   ================    CUDA    ================
find_package(CUDA QUIET REQUIRED)
#set(CUDA_TOOLKIT_ROOT_DIR "/usr/local/cuda")
if (CUDA_FOUND)
    include_directories(${CUDA_INCLUDE_DIRS})
    link_directories(${CUDA_LIBRARIES})
    set(CMAKE_CUDA_STANDARD 14)
    set(CMAKE_CUDA_FLAGS	"${CMAKE_CUDA_FLAGS}	-Xcompiler=-Wall")
    set(CMAKE_CUDA_FLAGS_DEBUG "${CMAKE_CUDA_FLAGS_DEBUG} -g")
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};
        -O3 -Wall -gencode arch=compute_61,code=sm_61;
        --std=c++14
    )

    message(STATUS "CUDA: FOUND on ${PROJECT_NAME}")
else()
    message(STATUS "CUDA: NOT FOUND on ${PROJECT_NAME}")
endif()


#   ================    OpenMP    ================
find_package(OpenMP QUIET)

if (OPENMP_FOUND)
    message(STATUS "OPENMP: FOUND on ${PROJECT_NAME}")
    #set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    #set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
else()
    message(STATUS "OPENMP: NOT FOUND on ${PROJECT_NAME}")
endif()


set(SRCS
    kernel/kernel.cu
    kernel/matrix.cu
	)
	
set(HEADERS
    kernel/kernel.cuh
    kernel/kernel.h
    kernel/kernel.hpp
    kernel/matrix.cuh
    kernel/matrix.h
    kernel/matrix.hpp
    kernel/matrix.tpp
    )

#cuda_add_library(my_cuda STATIC ${SRCS} ${HEADERS} OPTIONS -arch sm_75)
add_library(my_cuda STATIC ${SRCS} ${HEADERS})
include_directories("kernel/")
target_compile_features(my_cuda PUBLIC cxx_std_11)
set_target_properties(my_cuda PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(my_cuda PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_property(TARGET my_cuda PROPERTY CUDA_ARCHITECTURES 61)


if(APPLE)
  # We need to add the path to the driver (libcuda.dylib) as an rpath, 
  # so that the static cuda runtime can find it at runtime.
  set_property(TARGET my_cuda 
               PROPERTY
               BUILD_RPATH ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})
endif()

if(UNIX)
  # Help the shared cuda runtime find libcudart as it is not located
  # in a default system searched location
  set_property(TARGET my_cuda PROPERTY BUILD_RPATH ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})
endif()

add_executable(vec1d vec1d.c)
target_link_libraries(vec1d ${CUDA_LIBRARIES})
target_link_libraries(vec1d my_cuda)
set_property(TARGET vec1d PROPERTY CUDA_ARCHITECTURES 61)

add_executable(vec2d_mult vec2d_mult.c)
if (OPENMP_FOUND)
    target_link_libraries(vec2d_mult ${CUDA_LIBRARIES} OpenMP::OpenMP_C)
else()
target_link_libraries(vec2d_mult ${CUDA_LIBRARIES})
endif()
target_link_libraries(vec2d_mult my_cuda)
set_property(TARGET vec2d_mult PROPERTY CUDA_ARCHITECTURES 61)


add_executable(vec2d_add vec2d_add.c)
if (OPENMP_FOUND)
    target_link_libraries(vec2d_add ${CUDA_LIBRARIES} OpenMP::OpenMP_C)
else()
target_link_libraries(vec2d_add ${CUDA_LIBRARIES})
endif()
target_link_libraries(vec2d_add my_cuda)
set_property(TARGET vec2d_add PROPERTY CUDA_ARCHITECTURES 61)

add_executable(vec2d_mult2 vec2d_mult.cpp)
if (OPENMP_FOUND)
    target_link_libraries(vec2d_mult2 ${CUDA_LIBRARIES} OpenMP::OpenMP_CXX)
else()
target_link_libraries(vec2d_mult2 ${CUDA_LIBRARIES})
endif()
target_link_libraries(vec2d_mult2 my_cuda)
set_property(TARGET vec2d_mult2 PROPERTY CUDA_ARCHITECTURES 61)


#cuda_add_executable(main main.cpp)
#target_link_libraries(main ${CUDA_LIBRARIES})
#target_link_libraries(main kernel_obj)

#add_executable(main main.cpp kernel.cu)
#target_compile_features(main PUBLIC cxx_std_11)
#set_target_properties(main PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
#set_property(TARGET main PROPERTY CUDA_ARCHITECTURES 75)
#target_link_libraries( main  PRIVATE ${CUDA_LIBRARIES})



#add_library(my_crypto_static STATIC ${SRCS} ${HEADERS})
#add_library(my_crypto SHARED ${SRCS} ${HEADERS})
#target_link_libraries(my_crypto PRIVATE ssl crypto)
