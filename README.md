# MiniVideoCard

Generate 2D images from 3D models in CPU / RAM using the general principle behind GPU-based video adapters. 

The current implementation is C++ based, runs sequentially (i.e. no threads or parallel processing), and generates images as large as your chosen platform and RAM size allows them to be.

Developers using this library create and specify their own vertex and fragment shaders to the rasterizer component, allowing them to control in a precise way how polygons and pixels are written into the image. For this task, the library provides several basic (2, 3, and 4-dimensional) Vector and Matrix implementations, as well as a 2-dimensional Texture class with support for mipmaps and a separate Sampler class.

Pros:
* **Portable**. Can be included in any projects with minimal-to-no dependencies on any extra language features.
* **No dependencies on GPU**-based hardware or anything other than what the CPU already provides.
* Uses **double-precision floating point values** in its calculations, as well as in the final image, thus offering an unprecedented degree of precision in its calculations.
* **Shaders** for the rasterizer are written using the **same language** used by the rasterizer itself, offering a wide degree of flexibility on their creation.

Cons:
* **Slow**. Most calculations are made on CPU using floating-point values, which tend to be generally slow on existing CPUs today.
* **Elevated memory usage**. Focus for the implementation was on correctness and precision, rather than speed or RAM usage.
* **Non-customizable image formats**. The color buffer is always in RGBA format, with floating-point values for each component.

The library is distributed as source code-only. A test project is included, showing how to configure and use the rasterizer, as well as how to read the resulting image to save it to a BMP file. A few simple vertex and fragment shaders are also included, exemplifying its creation and usage.

Copyright (c) 2017 Heriberto Delgado.
