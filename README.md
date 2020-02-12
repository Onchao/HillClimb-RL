# HillClimb-RL

A game based on popular "Hill Climb Racing".\
In contrary to original version a car can be controlled by a **human player** and an **AI agent**.

An agent learns how to drive using the **Q-Learning** algorithm.

There are two types of maps: a flipped Gaussian curve and randomly generated.

An application can be controlled with a command line and standard keyboard input.\
Available commands are displayed in the terminal after starting a program. Later after typing executing `help`.

In my project I used following libraries and published source code contains them: `OpenGL`, `GLFW`, `glad` `stb_image.h`, `FreeType`. Fragments of code are from https://learnopengl.com/

To start an application run following file:\
`Release/HillClimbReinforcementLearning.exe`

![Screen from game](/graphics/rl.png)
