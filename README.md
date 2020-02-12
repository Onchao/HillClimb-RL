# HillClimb-RL

A game based on popular "Hill Climb Racing".\
In contrary to original version a car can be controlled by a **human player** and an **AI agent**.

An agent learns how to drive using the **Q-Learning** algorithm. Maps are difficult enough so that they can't be driven by just accelerating right. There are two types of them: a flipped Gaussian curve and randomly generated. Of course, randomly generated maps are different every time.

An application can be controlled with a command line and standard keyboard input.\
Available commands are displayed in the terminal after starting a program. Later after typing executing `help`.

In my project I used following libraries and published source code contains them: `OpenGL`, `GLFW`, `glad` `stb_image.h`, `FreeType`. Fragments of code are from https://learnopengl.com/

To start an application run following file:\
`Release/HillClimbReinforcementLearning.exe`


![Screen from the game](/graphics/rl.png)
Screen from the game
