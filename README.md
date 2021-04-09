# raytracer

Implements a raytracer based on the book [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html). 

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
raytracer $ mkdir build
raytracer $ cd build
raytracer/build $ cmake -G "Visual Studio 16 2019" ..
raytracer/build $ start CS312-Raytracer.sln
```

Your solution file should contain multiple projects, such as `gradient` and `normals`.
To run from the git bash command shell, 

```
raytracer/build $ ../bin/Debug/gradient
raytracer/build $ ../bin/Debug/normals
```

*macOS*

Open terminal to the directory containing this repository.

```
raytracer $ mkdir build
raytracer $ cd build
raytracer/build $ cmake ..
raytracer/build $ make
```

To run each program from build, you would type

```
raytracer/build $ ../bin/gradient
raytracer/build $ ../bin/normals
```

Running Tips:\
In camera.h there's a boolean variable blur. When running without defocus blur, make blur = false, else, make blur = true.\
In art_work.cpp, there is an integer variable number. If generating test_triangle.png, make number = 1, if generating test_planes.png, make number = 2, if generating the unique image scene, make number = 3.


Features Implemented:
1. For unique feature, I implemented defocus blur.
2. For unique picture, I created a scene with the following features:\
   a. Changed background color.\
   b. Arranged spheres in particular shape, and used different materials and colors to create reflection effects.\
   c. Used triangles to make two pyramid shapes with phong and glass materials.\
   d. I used all of my primitives: planes, triangles, spheres.\
   e. There is optional defocus blur.

Images:\
All images generated for this assignment are in the images/ directory. I'm skipping the basic ones here.
1. materials.png :\
![alt text](https://github.com/JiangxueHan529/raytracer/blob/main/images/materials.png)
2. materials with defocus blur:\
![alt text](https://github.com/JiangxueHan529/raytracer/blob/main/images/blurred_materials.png)
3. testing triangles:\
![alt text](https://github.com/JiangxueHan529/raytracer/blob/main/images/test_triangle.png)
4. testing planes:\
![alt text](https://github.com/JiangxueHan529/raytracer/blob/main/images/test_plane.png)
5. unique image:\
![alt text](https://github.com/JiangxueHan529/raytracer/blob/main/images/art_work.png)
6. unique image with defocus blur:\
![alt text](https://github.com/JiangxueHan529/raytracer/blob/main/images/art_work_blurred.png)

