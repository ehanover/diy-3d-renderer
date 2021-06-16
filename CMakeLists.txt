# Basic project with library: https://cmake.org/examples/

cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 11)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_options(-Wall)
endif()
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

project(diy-opengl VERSION 2.0.0)

#find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS})
#target_link_libraries(diy-opengl ${SDL2_LIBRARIES})

# Engine
add_subdirectory(diy-engine)

# Game (engine-test)
add_subdirectory(demos/engine-test)
install(FILES "build/demos/engine-test/engine-test" DESTINATION "build" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
install(DIRECTORY "demos/engine-test/assets" DESTINATION "build")

# Game (infinite-runner-game)
add_subdirectory(demos/infinite-runner-game)
install(FILES "build/demos/infinite-runner-game/infinite-runner-game" DESTINATION "build" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
install(DIRECTORY "demos/infinite-runner-game/assets" DESTINATION "build")


