# **FPS**

FPS made using the previous engine for the 3D Platformer.

**/!\ This engine is not the most optimized one nor the most efficient one. It supports only .obj models, .mtl materials and a custom scene format. /!\\**

# Informations
The program runs in Debug or Release, platform x64. 
You need to put the files :
- irrKlang.dll 
- ikpMP3.dll
- ikpFlac.dll <br>
in the folder x64/Release (or x64/Debug), they can be found on sirius : RMLG/BIN.

The resources (obj, textures, mtl, etc) are in the zip file on sirius : RMLG/BIN/resources.

# Controls
- **WASD/ZQSD keys** : Move foward, backward, left and right.
- **Space** : Jump.
- **Escape** : Pause Screen.
- **C** : Open inspector.

# Engine Features
* The Game Engine uses a powerful leak-free Component System
* The Game Engine is formed by the Render Engine and the Physic Engine
* The Game Engine supports UI system and button callback

# In-Game Features
* The player can launch the game from the main menu
* The player can exit the game from the main menu
* The player can move in a 3D-environment while being in game
* The player can pause the game
* The player can exit the game from the pause screen
* The player can go back to the main menu from the pause screen

***Annexes***
===
- Link to the UML:
https://miro.com/welcomeonboard/Y1VWWjN5ZUptejZDWVhxWkR2SElZZjRzR2RXaWZjMUpmUG5yZ05wUExSZmVPWFBpdDg3bXFPeklyVUpkRDdlaHwzMDc0NDU3MzU3NzMyNDc2MDQ5?invite_link_id=717859921437

***Exemples***
===

Here are some examples of what you can see in the game. If you want more you can go to the folder annexes/screenshots.

<div style="text-align:center">

![Menu](screenshots/menu.png)
Screenshot of the main menu

![Pause](screenshots/pause.png)
Screenshot of the pause screen

![Win](screenshots/win.png)
Screenshot of the win screen

<div style="text-align:left">

***Work in progress and future features***
===
WIP
---
- Shadow mapping

Next features
---
- Particle system
- Bloom

***Third-party programs and libraries***
===

# Download links

irrKlang
---
https://www.ambiera.com/irrklang/

stb
---
https://github.com/nothings/stb

GLFW
---
https://www.glfw.org/

Glad
---
https://glad.dav1d.de/

***References***
===
OpenGL:
---
- Gives the different functions of OpenGL:
http://docs.gl/
- Ditto:
https://www.khronos.org/opengl/

C++:
---
- C++ references: https://en.cppreference.com/