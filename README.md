# C-RayMarch-Quaternion-Renderer
A renderer with raymarching capabilities and a player camera control scheme built around quaternion rotations.

This project requires a LINUX or UNIX-compatible OS, as well as the libraries:

* libncursesw5 or later to run, libncursesw5-dev or later to compile.
* xpadneo driver for xbox controller support. Any other driver which creates the file /dev/input/js0 should work too. xpadneo supports bluetooth which is nice.


to compile use the compile line:
`gcc main.c quaternion.c vec3.c spaceship.c world.c object.c drivers.c -o main -lm -lncursesw -lpthread`

## DESIGN FAQ
### Q: The screen is a terminal emulator! Where is the GLSL code? where is the SDL windowing code??
A: I personally chose the terminal aesthetic because I wanted to blend new and old, mixing state of the art graphical rendering algorithms with old teletype games like dwarf fortress or MUDs. My goal is to use this renderer to create a small, socket based multiplayer world right in your terminal.


### Q: Okay but then don't you have to write the entire fragment and vertex shader yourself?
A: Yep! And that's kind of the point. openGL abstracts away what actually happens in the graphics pipeline, mostly by being helpful and parallelizing pixel shaders, but I wanted to experience writing every vector manipulation required by hand. It was technical and mathematical work, but it's given me a new appreciation for every step of consideration in a raymarched pipeline down to the simple mathematical operations.

## Screenshots:
![A Sphere](https://github.com/rosesilver/C-RayMarch-Quaternion-Renderer/blob/master/sphere.png)
![Many Spheres](https://github.com/rosesilver/C-RayMarch-Quaternion-Renderer/blob/master/spheres.png)
![A Capsule](https://github.com/rosesilver/C-RayMarch-Quaternion-Renderer/blob/master/capsule.png)

## Videos:
![Animated Scene](https://github.com/rosesilver/C-RayMarch-Quaternion-Renderer/blob/master/scene.mkv)

This video shows the full resolution available to my computer to show off shadows and animation, but because my workstation is a 2013 macbook Air, it is horrendously slow framerate. Any newer generation computer should be able to handle running this just fine


![Joystick Use](https://github.com/rosesilver/C-RayMarch-Quaternion-Renderer/blob/master/joystick.mkv)

This video is lower resolution, for framerate, and shows the player flying through space. The player is controlled by XBOX controller, and the math is done with quaternions to make yaw, roll, and pitch simple to calculate. Quaternions are my newest friend

