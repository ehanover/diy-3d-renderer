# diy-engine
# Finding SDL: https://stackoverflow.com/questions/28395833/using-sdl2-with-cmake

set(MyLibName "diy-engine")

#FILE(GLOB SOURCES src/*.cpp)
#add_library(diy-engine ${SOURCES})

# Add source files
set(MySourcesDir "src")
set(MySources ${MySourcesDir}/camera.cpp ${MySourcesDir}/fileloader.cpp ${MySourcesDir}/light.cpp ${MySourcesDir}/mymatrix.cpp ${MySourcesDir}/myvector.cpp ${MySourcesDir}/object.cpp ${MySourcesDir}/renderer.cpp)
#message(STATUS ${MySources})
add_library(${MyLibName} ${MySources})

# Include SDL2
find_package(SDL2 REQUIRED)
target_include_directories(${MyLibName} PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(${MyLibName} PRIVATE ${SDL2_LIBRARIES})

# Include engine headers
target_include_directories(${MyLibName} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

