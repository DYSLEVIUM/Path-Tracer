# Path Tracer In One Weekend

## 1. Output an Image

Here we are using ppm format to output the image.

![alt text](assets/fig-1.01-ppm.jpg)

### 1.1 Creating the image

`$ a.exe > img.ppm` // this is not used in the code as it would be tedious, the code automatically makes the image file

## 2. Rays, Camera and Background

### 2.1 Ray

Let’s think of a ray as a function **P**(t)=**A**+_t_ **b**. Here **P** is a 3D position along a line in 3D. **A** is the ray origin and **b** is the ray direction. The ray parameter _t_ is a real number. Plug in a different t and **P(t)** moves the point along the ray. Add in negative _t_ values and you can go anywhere on the 3D line. For positive _t_, you get only the parts in front of **A**, and this is what is often called a half-line or ray.

![alt text](assets/fig-1.02-lerp.jpg)

### 2.2 Sending Rays Into the Scene

At the core, the ray tracer sends rays through pixels and computes the color seen in the direction of those rays. The involved steps are:

    1. Calculate the ray from the eye to the pixel.

    2. Determine which objects the ray intersects.

    3. Compute a color for that intersection point.

In addition to setting up the pixel dimensions for the rendered image, we also need to set up a virtual viewport through which to pass our scene rays. For the standard square pixel spacing, the viewport's aspect ratio should be the same as our rendered image. We'll just pick a viewport two units in height. We'll also set the distance between the projection plane and the projection point to be one unit. This is referred to as the _**focal length**_, not to be confused with _**focus distance**_.

I’ll put the cameraat (0,0,0). I will have the y-axis go up, and the x-axis to the right. In order to respect the convention of a right handed coordinate system, into the screen is the negative z-axis. I will traverse the screen from the lower left hand corner, and use two offset vectors along the screen sides to move the ray endpoint across the screen. Note that I do not make the ray direction a unit length vector because I think not doing that makes for simpler and slightly faster code.

![alt text](assets/fig-1.03-cam-geom.jpg)

The `ray_color(ray)` function linearly blends white and blue depending on the height of the _**y**_ coordinate after scaling the ray direction to unit length (so **−1.0 < y < 1.0**). Because we're looking at the _**y**_ height after normalizing the vector, you'll notice a horizontal gradient to the color in addition to the vertical gradient.

Then that is scaled to **0.0 ≤ t ≤ 1.0**. When **t = 1.0** it will blue. When **t = 0.0** it will be white. In between, it will be a blend. This forms a _**linear blend**_, or _**linear interpolation**_, or _**lerp**_ for short, between two things. A lerp is always of the form

_**blendedValue = (1 − t)⋅startValue + t⋅endValue**_,

with _**t**_ going from zero to one. In our case this produces:

![alt text](assets/img-1.02-blue-to-white.png)

All of the images and text are are taken from _**"Ray tracing in One weekend"**_.
_**https://raytracing.github.io/books/RayTracingInOneWeekend.html**_
