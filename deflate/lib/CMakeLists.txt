file(
	GLOB_RECURSE
	SRC_FILES
	CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(deflate STATIC ${SRC_FILES})
target_include_directories(deflate PRIVATE ${CMAKE_SOURCE_DIR}/include)
