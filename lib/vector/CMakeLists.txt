cmake_minimum_required(VERSION 3.11)

project(vector C CXX)

set(SRCS
	vector.cpp
	vector_avx.cpp
	)
	
set(HEADERS
	vector.hpp
	vector_imp.hpp
	vector_avx.hpp
	)

#add_library(math_static STATIC ${SRCS} ${HEADERS})
add_library(vector ${SRCS} ${HEADERS})
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.16.0")
    set_target_properties(vector PROPERTIES UNITY_BUILD ON)
	#target_precompile_headers(vector PUBLIC ${HEADERS})
    message(STATUS "CMAKE_UNITY_BUILD : 1 on ${PROJECT_NAME}")
else()
    message(STATUS "CMAKE_UNITY_BUILD : 0 on ${PROJECT_NAME}")
endif()

#install (TARGETS math DESTINATION bin)
#install (FILES math.h DESTINATION include)

set_target_properties(${PROJECT_NAME} PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
)

install(TARGETS ${PROJECT_NAME} 
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # Nedd more 1 target
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # Nedd more 1 target
#    PRIVATE_HEADER DESTINATION source
    PUBLIC_HEADER DESTINATION "include/${PROJECT_NAME}"
)