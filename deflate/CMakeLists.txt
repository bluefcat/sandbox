cmake_minimum_required(VERSION 3.11)

project(
	Deflate
	VERSION 0.0.1
	DESCRIPTION "deflate program"
	LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
	googletest	
	URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_subdirectory(lib)
add_executable(run main.cpp)
target_link_libraries(run PUBLIC deflate)
target_compile_options(run PUBLIC --std=c++20 -g)

enable_testing()

add_executable(
	tester tester.cpp 
)

target_link_directories(
	tester PUBLIC ${CMAKE_SOURCE_DIR}/build/_deps/gtest-src/googletest/include
)

target_link_directories(
	tester PUBLIC ${CMAKE_SOURCE_DIR}/build/_deps/googletest-src/googletest/include
)
target_link_libraries(
	tester
	GTest::gtest_main
)

target_link_libraries(
	tester
	deflate
	)

target_compile_options(tester PUBLIC -std=c++20 -pthread)
gtest_discover_tests(tester)

