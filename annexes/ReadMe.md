# **Scripting System**
##### by Rémi and Jarod

# Description
This project is made to allow the user to change the game objects' behavior with Python script. They would be able to run specific scripts for each game object such as the player object.
 
# Informations
The program only runs in Release because of a missing Python library, platform x64.\
You need to put the Python scripts (they are in REMOD/BIN/resources/scripts) and the resources folder next to the .exe file.
The solution will only compile with C++ 20 standard (latest)

The resources (obj, textures, mtl, etc) are in the zip file on sirius : REMOD/BIN/resources.

The screenshots for the .md files are in Sirius.
 
# Controls
- **C** : Open inspector, and the interfaces of the Managers.
- **F5** : Hot reload the scripts.
 
# Engine Features
- The Game Engine reads Python scripts
- The scripts have access to the modules:
    - engine that contains Transform, Rigidbody, GameObject, Component and Object
    - debug to show informations and errors
- The scripts can be reloaded without restarting the program by pressing one button, either on the keyboard or on the inspector
- The player object will by default load a Python scripts named player_stats.py in order to get scripted values (for movement speed, etc)
 
***Examples***
===
 
Here are some examples of what you can see in the engine. If you want more you can go to the directory annexes/screenshots.
 
<div style="text-align:center">

![Script](screenshots/python_script.png)\
What a Python script looks like
 
![Debug](screenshots/python_debug.png)\
Testing the debug module
 
![Console](screenshots/python_console.png)\
Result
 
<div style="text-align:left">
 
***Work in progress and future features***
===
WIP
---
- Instance script: When a script is made, it must have a class with the same name of the file to create an instance of it
- Interfacing ScriptComponent class: have access to its functions from Python scripts
- Module system supports all managers
 
Next features
---
- Script inheritage: Python scripts will be inherited from Component class to access to the different functions of it
 
***Known bugs***
===
- The instance script (where each script creates an instance of a class) system breaks the global script system
- Only the first line of a method from an instance is called
 
***Third-party programs, libraries and resources***
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
 
Python
---
https://www.python.org/
 
Sponza
---
https://github.com/jimmiebergmann/Sponza 
 
***References***
===
PyHelper:
---
- Helper library to facilitate the use of PyObject pointers and instance:
https://www.codeproject.com/Articles/820116/Embedding-Python-program-in-a-C-Cplusplus-code
 
Swig:
---
- http://www.swig.org/Doc1.3/Python.html#Python_nn4
- http://www.swig.org/Doc1.3/Library.html#Library 
 
Pybind11:
---
- https://github.com/pybind/pybind11
 
C++:
---
- C++ references: https://en.cppreference.com/