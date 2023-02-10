cmake_minimum_required(VERSION 2.8)

project(tada-hdt-entity)

# copied from  # https://stackoverflow.com/questions/31561309/cmake-warnings-under-os-x-macosx-rpath-is-not-specified-for-the-following-targe
# to remove the rpath warning
set(CMAKE_MACOSX_RPATH 1)

add_library(mylibs SHARED "./src/entity.cpp" "./src/graph.cpp" "./src/tnode.cpp")
#add_library(mylibs SHARED "./src/entity.cpp")


target_link_libraries(mylibs -lpthread) # for mac
target_link_libraries(mylibs -pthread) # for ubuntu
target_link_libraries(mylibs -lhdt)
target_link_libraries(mylibs -ltabularparser)
target_link_libraries(mylibs -leasylogger)
target_link_libraries(mylibs "-L/usr/local/lib/")
target_link_libraries(mylibs "/usr/local/lib/getoptpp/libgetopt_pp.a")


set(SOURCES
    src/main.cpp
    src/entity.cpp
    src/tnode.cpp
    src/graph.cpp)

set(HEADERS
    include/entity.h
    include/tnode.h
    include/graph.h)


# This is to get rid of the the c++11 warnings
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


include_directories("include" "/usr/local/include")

#Rename Library
set_target_properties(mylibs PROPERTIES OUTPUT_NAME "tada-hdt-entity")



add_executable("${PROJECT_NAME}" "${SOURCES}" "${HEADERS}")
target_link_libraries("${PROJECT_NAME}" mylibs)


# For the installation
include(GNUInstallDirs)
install(TARGETS mylibs DESTINATION lib)
install(FILES include/entity.h include/tnode.h include/graph.h DESTINATION include/tada-hdt-entity)


## copied from gtest guide https://google.github.io/googletest/quickstart-cmake.html
## GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)
##enable_testing()

set(TSOURCES
    src/tnode.cpp
    src/graph.cpp
    src/entity.cpp
    src/tests.cpp
)


set(others
    test_files/test1.csv
    test_files/test2.csv
    test_files/test3.csv
    test_files/test4.csv
    test_files/test5.csv
    test_files/test6.csv
    test_files/test7.csv
    test_files/test8.csv
    test_files/test9.csv
    test.ttl
)


add_executable(tests "${TSOURCES}" "${HEADERS}" "${others}")
#add_executable(tests "${TSOURCES}" "${HEADERS}")


target_link_libraries(tests /usr/local/lib/libgtest.a)
target_link_libraries(tests -lpthread) # for mac
target_link_libraries(tests -pthread) # for ubuntu
target_link_libraries(tests -lhdt)
target_link_libraries(tests -ltabularparser)
target_link_libraries(tests -leasylogger)
target_link_libraries(tests "-L/usr/local/lib/")
target_link_libraries(tests "/usr/local/lib/getoptpp/libgetopt_pp.a")




