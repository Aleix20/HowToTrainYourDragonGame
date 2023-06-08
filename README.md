3D game made with the following framework, based on how to train your dragon. 


Implementations discussed in the video:


-Character and dragon collisions

-World builder

-Player and dragon movement

-Saving and reading in a file 

-Minimal interactions with the characters in the scene (audios by distance and animations)

-3 Missions


-Tutorial


We have missed to put a final boss, so that the mini-games have more sense.

Controls
------------------------------
Player movement: AWSD, shift run
Dragon movement: AWSD and ArrowUp ArrowDown up and down ArrowLeft shoot (mission2 only)
Character/dragon interaction: F

World Editor
---------------------------
Select which entities to modify and where to save props: z
Select which entity to build: backward , forward .
Select entity to modify: click on mesh
Add selected object: 2
Rotate an object: -+
Raise and lower an object: F2 and F3
Scale an object: F4 and F5
Delete selected object: F6
Move object: arrows
Rotate vertically: K L 
Save changes: g


VIDEO: https://www.youtube.com/watch?v=ICdxsoUyOEo

# TJE Framework #

TJE Framework is a C++ layer on top of SDL and OpenGL to help create games or visual applications.
It only provides the basic GPU abstraction (Meshes, Textures, Shaders, Application).
It has been used for many years in the Videogame's development courses I give at the Universitat Pompeu Fabra.

It contains the basics to do something nice:
- Mesh, Texture, Shader and FBO classes
- Vector2,Vector3,Vector4,Matrix44 and Quaternion classes
- Meshes have a ray-mesh and sphere-mesh collision method (thanks to library Coldet)
- Parser to load OBJ, PNG and TGA. Re-stores meshes in binary for faster load.
- Supports skinned animated meshes using own format (you must use a web tool to convert them).

Enough to make gamejam games pretty easily.


## Installation ##

It comes with a Visual Studio solution, but the code should compile in any platform that supports SDL.


## Compiling in Linux

Remember to install the libraries:

sudo apt-get install libsdl2-dev freeglut3 freeglut3-dev

and then just use the makefile:

make






