# DIY-OpenGL

A toy 3D renderer built without any 3D graphics libraries. Every pixel is rendered from scratch!

**Features I've implemented:** custom matrix and vector classes, object transformations, projection+perspective projections, camera movement, triangle rasterization, normal creation, backface+frustum culling, STL importing, ambient+diffuse lighting, z-buffering, hacky vertex coloring

**Features I want to implement:** object texturing, specular lighting (Phong), support for multiple lights, multithreading, background images from a file, text rendering, general optimizations

**Current problems:** shaders should belong to objects, renderer class is too big, unclear standard for individually coloring vertices, overall too slow when using higher resolutions

<br>

```
sudo apt install libsdl2-2.0-0 libsdl2-dev cmake

git clone https://github.com/ehanover/diy-opengl.git
cd diy-opengl
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make && cmake --install . --prefix ".."
./infinite-runner
```

<br>
<br>

Resources I've used:
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
