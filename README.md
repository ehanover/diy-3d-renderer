# DIY-OpenGL

My exploration of 3D rendering, without OpenGL or any other 3D graphics library. 

```
sudo apt install libsdl2-2.0-0 libsdl2-dev

git clone https://github.com/ehanover/diy-opengl.git
cd diy-opengl
make
make install
./diy-opengl
```

<br>

**Features I've implemented:** custom matrix and vector classes, some object transformations, projection+perspective projections, camera movement, triangle rasterization, normal creation, backface+frustum culling, STL importing, ambient+diffuse lighting, z-buffering

**Features I want to implement:** FPS upper bounding, complete object transformations, texturing, specular lighting (Phong), support for multiple lights, object coloring, multithreading (is this doable?), general optimization

**Current problems:** common-edge rasterization holes, unclear standard for individually coloring vertices

<br>

Resources I've used
* Graphics overview: https://en.wikipedia.org/wiki/Graphics_pipeline
* Coordinate systems: https://learnopengl.com/Getting-started/Coordinate-Systems
* Projection and perspective, transformations: http://codeofthedamned.com/index.php/3d-projection-and-matrix-transforms
* Transformations of normals: https://www.scratchapixel.com/lessons/3d-basic-rendering/transforming-objects-using-matrices
* Rasterization and interpolation: https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
* Fragment interpolation for coloring: https://paroj.github.io/gltut/Illumination/Tut10%20Fragment%20Lighting.html
* Lighting concepts: https://learnopengl.com/Lighting/Basic-Lighting
* STL files: http://www.jgxsoft.com/examples/STL%20Reader/STL%20Reader.html
