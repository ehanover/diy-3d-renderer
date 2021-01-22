# Basic project with library: https://cmake.org/examples/

cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 11)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_options(-Wall)
endif()

project(diy-opengl VERSION 2.0.0)

#find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS})
#target_link_libraries(diy-opengl ${SDL2_LIBRARIES})

# Engine
add_subdirectory(diy-engine)

# Game (engine-test)
add_subdirectory(games/engine-test)
install(FILES "build/games/engine-test/engine-test" DESTINATION "build")
install(DIRECTORY "games/engine-test/assets" DESTINATION "build")

# Game (infinite-runner)
#add_subdirectory(games/infinite-runner)
#install(FILES "build/games/infinite-runner/infinite-runner" DESTINATION "build")
#install(DIRECTORY "games/infinite-runner/assets" DESTINATION "build")


