# DIY-3D-Renderer

A toy 3D renderer built without any 3D graphics libraries. Every pixel is drawn from scratch!

I originally wanted to learn how to use OpenGL to create 3D graphics, but I had a very hard time understanding the fundamental 3D concepts like normals, projections, and lighting. Making my own simple 3D renderer taught me so much about these tricky topics, so hopefully learning OpenGL will be easier now. Plus, this project was super fun to work on!

To show what the engine can do, I made a simple demo game under ```demos/infinite-runner-game```. Try to stay alive as long as you can by jumping and moving to avoid the spikes.

<a href="https://github.com/ehanover/diy-3d-renderer/blob/main/screenshots/infinite-runner-game.mp4?raw=true">
  <img src=./screenshots/infinite-runner-game.gif width=480 title="Infinite Runner demo game"/>
</a>
<br>

Running the demo:
```
sudo apt install libsdl2-2.0-0 libsdl2-dev cmake

git clone https://github.com/ehanover/diy-3d-renderer.git
cd diy-opengl
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && make && cmake --install . --prefix ".."
./infinite-runner-game
```

<br>

**Features I've implemented:** custom matrix and vector classes, object transformations, projection+perspective matrices, camera movement, triangle rasterization, normal creation, backface+frustum culling, STL importing, ambient+diffuse lighting, z-buffering, hacky vertex coloring

**Features I want to implement:** object texturing, specular lighting (Phong), support for multiple lights, multithreading, background images from a file, text rendering, general optimizations

**Current problems:** overall very slow and unoptimized, shaders should belong to objects, renderer class is too big, no clear file standard for individually coloring vertices

<br>

Useful resources
* Graphics overview: https://en.wikipedia.org/wiki/Graphics_pipeline
* Coordinate systems: https://learnopengl.com/Getting-started/Coordinate-Systems
* Projection and perspective, transformations: http://codeofthedamned.com/index.php/3d-projection-and-matrix-transforms
* Transformations of normals: https://www.scratchapixel.com/lessons/3d-basic-rendering/transforming-objects-using-matrices
* Rasterization and interpolation: https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
* Fragment interpolation for coloring: https://paroj.github.io/gltut/Illumination/Tut10%20Fragment%20Lighting.html
* Lighting concepts: https://learnopengl.com/Lighting/Basic-Lighting
* SDL setup basics: https://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
* STL files: http://www.jgxsoft.com/examples/STL%20Reader/STL%20Reader.html
* Profiling with gprof: http://www.math.utah.edu/docs/info/gprof_toc.html
