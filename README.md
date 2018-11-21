This was my final project for the Advanced Computer Graphics course at UCSD wherein I programmed realtime VFX. I looked at tech videos and forum posts for the game Star Citizen, and implemented their approach to fluid visualization. See this video for the results: https://www.youtube.com/watch?v=20xHe2O7mKk The floating crosses are watermarks due to FumeFX not having a student license.

Project milestones:
• Simulated and rendered animation frames in 3ds Max with the FumeFX plugin.
• Made a program that further processed the rendered frames to get alpha, diffuse, and normal maps.
• Generated optical flow maps with Slate Editor.
• Used OpenGL and C++ to visualize the animation, with a shader that uses the optical flow maps to blend between frames.
