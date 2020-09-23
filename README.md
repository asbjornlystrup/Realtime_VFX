## DESCRIPTION

This was my final project for the Advanced Computer Graphics course at UCSD wherein I programmed realtime VFX. I looked at tech videos and forum posts for the game Star Citizen, and implemented their approach to fluid visualization. Click here for a video: https://www.youtube.com/watch?v=20xHe2O7mKk. The floating crosses are watermarks due to FumeFX not having a student license.

Project milestones:

• Simulated and rendered animation frames in 3ds Max with the FumeFX plugin

• Made a program that further processed the rendered frames to get alpha, diffuse, and normal maps

• Generated optical flow maps with Slate Editor

• Used OpenGL and C++ to visualize the animation, with a shader that uses the optical flow maps to blend between frames

## PRE-BUILT BINARIES

There exists a pre-built binary for Windows, project/CSE163_A3.exe.

## BUILD INSTRUCTIONS: LINUX

Install dependencies:

```
    $ sudo apt-get install -y libfreeimage3 libfreeimage-dev libglfw3 libglfw3-dev glew-utils mesa-commmon-dev freeglut3 libglu1-mesa
```

Then build with make.
