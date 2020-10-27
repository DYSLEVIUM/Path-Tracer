# Path Tracer Renderer

## 1. Output an Image

Here we are using ppm format to output the image.

![alt text](assets/fig-1.01-ppm.jpg)

### 1.1 Creating the image

`$ a.exe > img.ppm`

## 2. Rays, Camera and Background

### 2.1 Ray

Let’s think of a ray as a function **P**(t)=**A**+_t_ **b**. Here **P** is a 3D position along a line in 3D. **A** is the ray origin and **b** is the ray direction. The ray parameter _t_ is a real number. Plug in a different t and **P(t)** moves the point along the ray. Add in negative _t_ values and you can go anywhere on the 3D line. For positive _t_, you get only the parts in front of **A**, and this is what is often called a half-line or ray.

![alt text](assets/fig-1.02-lerp.jpg)

All of the images and text are are taken from _**"Ray tracing in One weekend"**_.
_**https://raytracing.github.io/books/RayTracingInOneWeekend.html**_
