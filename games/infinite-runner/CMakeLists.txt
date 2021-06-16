# infinite-runner
# Using "include" folder from a library in the same project: https://stackoverflow.com/a/47060462

set(MyGameName "infinite-runner")
set(MySources main.cpp ground.cpp healthbar.cpp player.cpp)

# Add source files
add_executable(${MyGameName} ${MySources})

# Include SDL2
find_package(SDL2 REQUIRED)
target_include_directories(${MyGameName} PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(${MyGameName} PRIVATE ${SDL2_LIBRARIES})

# Include engine headers
set(MyEngineInclude "${PROJECT_SOURCE_DIR}/diy-engine/include")
target_include_directories(${MyGameName} PUBLIC ${MyEngineInclude})

# Link engine with this game
target_link_libraries(${MyGameName} PUBLIC diy-engine)
