# Mandelbrot-Fractal
This is a mandelbrot fractal plotter written in C++ using the SDL2 graphics library. The mandelbrot set is one of the most intriguing mathematical
concepts you'll ever see.

# Key bindings
* **WASD** to pan around
* **+/-** to zoom in/out
* **C** to capture screenshot

# A bit of a you-can-skip stuff
So this was a good bout of coding in a while.
There were two basic challenges in coding this: 
Firstly, optimizing the rendering routine and,
secondly, the Mandelbrot algorithm used to calculate number of iterations.

Now the rendering needs optimization because your'e basically updating each pixel manually on the screen. So that means your'e doing W * H draw calls every frame.
But the golden rule of graphics optimization: "Minimize draw calls". So what I did is create a single pixel buffer and first update it using the mandelbrot
algorithm, then push the whole texture to GPU and draw it. Seriously, it took some time coding this and the code is not pretty... 

Now I couldn't do much for optimizing the mandelbrot algorithm itself though I did parallelize the routine. Basically I split the whole screen pixels into 4
horizontal parts, and update each part on a separate thread (my first use of multi-threading...). The speed-ups were impressive (about 2-4x). Now keep in mind
the program is still slow when looking at dense parts. There's not much anyone can do except using a better CPU. Also this also shows why C++ was the obvious
choice for this stuff, no other language can do the insane level of optimization C++ does. Seriously, try updating each pixel separately every frame using any other 
language.

# Screenshots

![img1](https://i.ibb.co/vVQW9Dg/ss-1594109298.png)
![img2](https://i.ibb.co/gz18sC5/ss-1594109309.png)
![img3](https://i.ibb.co/YRXdvS6/ss-1594109332.png)

