# **Resources Manager**

Resources Manager improvement made using the previous engine for the FPS.

**/!\ This engine is not the most optimized one nor the most efficient one. It supports only .obj models, .mtl materials and a custom scene format. /!\\**

# Informations
The program runs in Debug or Release, platform x64. 
You need to put the files :
- irrKlang.dll 
- ikpMP3.dll
- ikpFlac.dll <br>
in the folder x64/Release (or x64/Debug), they can be found on sirius : RSMNGR/BIN.

The resources (obj, textures, mtl, etc) are in the zip file on sirius : RSMNGR/BIN/resources.

# Controls
- **C** : Open inspector, and the interfaces of the Managers.

# Engine Features
* The Game Engine uses a powerful leak-free Resources Manager
* The Game Engine is formed by the Render Engine, the Physic Engine and a multi-treaded Resources Manager
* The game can reload a scene very quickly (instantly) thanks to the use of shared_ptr
* The game engine has a Benchmarking option to benchmark the load of a scene automatically

# Benchmark
For a scene using 221 Mo of resources: 13 different models, 81 different textures and 36 materials.
The engine can load in:

In Release
---
- With multithreading: 7 seconds.
- In monothread: 30 seconds.

In Debug
---
- With multithreading: 33 seconds.
- In monothread: 5 minutes and 30 seconds.

***Annexes***
===
- Link to the UML:
https://miro.com/welcomeonboard/Y1VWWjN5ZUptejZDWVhxWkR2SElZZjRzR2RXaWZjMUpmUG5yZ05wUExSZmVPWFBpdDg3bXFPeklyVUpkRDdlaHwzMDc0NDU3MzU3NzMyNDc2MDQ5?invite_link_id=717859921437

***Examples***
===

Here are some examples of what you can see in the game. If you want more you can go to the folder annexes/screenshots.

<div style="text-align:center">

![Menu](screenshots/sponza.png)
Screenshot of the Sponza loading its resources

![Pause](screenshots/benchmarker.png)
Screenshot of the benchmarker showing the loading statistics

<div style="text-align:left">

***Work in progress and future features***
===
WIP
---
- OBJ Parser group support

Next features
---
- Multi-threading physics

***Know bugs***
===
- Sometimes some textures do not load due to an issue with fopen

***Third-party programs, libraries and resoucres***
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

Sponza
---
https://github.com/jimmiebergmann/Sponza

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