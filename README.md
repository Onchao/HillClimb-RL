# HillClimb-RL

This project is inspired by the popular mobile game _Hill Climb Racing_, with a key difference: the car can be controlled by either a **human player** or an **AI agent**.

A key feature of the game is that **the map isn’t drivable by simply accelerating forward** - the car sometimes needs to drive backward to gain speed on downward slopes.

### Purpose
This project was developed to:
- Gain a deeper understanding of **low-level graphics rendering** by working directly with **OpenGL**, without relying on high-level graphics engines.
- Learn how textures, shaders, and vertices are processed and rendered as triangles on the GPU.
- Implement an algorithm that generates challenging yet drivable random maps for the car.
- Simulate basic physics, including collisions, gravity, and friction.
- Develop a Q-Learning AI agent capable of learning the map and driving efficiently.

### Controls
- The car can be controlled using the arrow keys.
- Additional commands for the AI agent are available through the terminal.

### Dependencies
This project utilizes several libraries to enhance OpenGL functionality:
- GLFW - Window and input management
- glad - OpenGL function loader
- stb_image.h - Image loading
- FreeType - Font rendering

Some code fragments have been adapted from [LearnOpenGL](https://learnopengl.com/)

<p align="center">
    <img src="/graphics/rl.png" alt>
    <em>A view of the game and the list of available commands. The green line marks the furthest point reached by the car.</em>
</p>
