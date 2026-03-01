##### Physics Simulations With OpenGL:
---

This repository is a take on simulating real life physics phenomeons 
using the high level graphics api known as opengl. It uses a basic
engine structure (specified below), in order to render different scenes
that each have their own objects and properties. Currently this project 
does not have any implementation, but eventually I plan to include the 
following physics simulations:

1.   2d Simulation of blackholes (COMPLETED)
2.   3d Simulation of blackholes
3.   2d Simulation of gravity/solar system
4.   3d Simulation of gravity/solar system
5.   2d simulation of some sort of a cloth/rope physics
6.   2d simulation of sand/grains, maybe make it 3d or something
7.   3d simulation of balls inside of a box (like a cube with a ball in it that you can shake)
8.   2d simulation of a pendulum or something of the sort
9.   2d simulation of a spring sort of an ordeal
10.  maybe do some fun shit with waves
11. try out some fluid simulation maybe

---

### Engine Architecture:

**Engine Object -**
The entire code base stems from a main engine class, this class holds different things.
For one, is a map of scene objects in refrence to a scene name. The scenes are the things
that focus on rendering things to the screen, handling input, etc. The game engine holds
these scene objects and is able to create, change between, or delete them. It also holds
the window object that is needed to display anything to the screen, and it intiates the
while loop that renders said things. It also has a pointer to a renderer and assets object,
which can be found below. 

**Scene Objects -**
The scene objects are the real meat of the engine. They handle the main logic of what each of
the different simulations does. It handles the draw calls to the renderer, as well as all of
the actual mathematic physics simulations. The scene objects inheret from a main scene object
class, which is a virtual abstract class that takes in the engine as the needed arguement 
in the constructor. This means that the scene can access the engine, and the engine can access
the scene. This is generally needed because the scene has to handle input that is sent to it
from the engine, and access certain variables for draw calls such as the window or deltatime. 
The scene also dictates the camera's view of itself. This means that it inherently holds a 
camera object, which more about can be seen below

**Renderer -**
The renderer object is really the main thing that brings the entire engine together. It handles
all of the draw calls that the scene makes for it. The renderer is a shared pointer that is created
in the init() function of the engine class. It is in the engine class and not the scene class because
it has to load all of the needed openGL data, such as the vertex array objects, vertex buffer data,
as well as the index buffer objects. All of which are sent over to the gpu and run in the shader programs
which are also specified and created in the renderer class. In the constructor, each shader is created
via the given path of the shader, and then all data for the needed shapes, such as cubes, spheres, rects
triangles etc is created. The renderer function also holds the projection, view, and model matrix, which
dictate the position and view of all the objects that it draws. The draw calls are given to the renderer
via the scene, and take position, color, rotation, and texture as given arguements. The render sends all
of this information to the shader, specifies which shader to use, and the actually draws all of the objects
to the scene.

**Camera -**
The camera object mainly handles the view matrix, allowing for the visual effect of a 3d camera that is able
to move and fly around. There is also a static 2d camera, that simply is created and does not move or take
in any input. The camera class itself is also abstracted out into a base class, and then derivied into
two class as specified before. The scene itself switches between these two class types as needed. The main
function of the camera is, using linear algebra and matrix math, to update the inner renderer class matrices.
It's a bit wonky given it also needs to accept input, but generally it is very simple and just specifes where
the "camera" is in opengl.

**Assets -**
The assets manager class is also very simple. It is too a pointer that lives in the game engine class. This is
because even though the textures are loaded per scene basis, it allows for the storage of these textures. Meaning
they only need to be created once, and can be reused in other scenes without loading. Allowing loading per scene
also reduces the overhead that is created whenever the engine object starts up. The assets class it's self is simply
a map of strings (names) to pointers of texture objects which are instantiated using a filepath. The texture class 
itself uses stb_image, a free image processing library that converts the image into data that opengl can actually use.
By doing so, youre able to instantiate a texture object in opengl, and bind to it the data that you get from the image.
The assets class is then able to retrive this texture class object via the given name, and pass it to the renderer which
can pass it to the shader. Allowing for textures to be drawn onto the objects in the shader.

**Actions -**
The action system is generally kind of spread all throughout the entire codebase. The input values from the press
of a key are simple int values in the glfw library (the library used to render the window). This means that we can set
up a basic map for whatever key we have, as an int value, and a string to whatever action we want this key to perform.
Such as the w key causing us to go forward. However, each scene might have different actions for the same key. meaning that
each scene object needs to have it's own key to string map, so it lives in the abstraction of the scene class. However,
each key can only be gotten from the window, which lives in the game engine. This means that the game engine needs to pass over
the keycode to the scene's map, and see if it has a registered input at that keycode. If it does not, then the input does
nothing, however, if it does, then we send the action name over to the currently loaded scene, alongside with information of
if the key was just pressed or released. Using this information the scene class can then handle input.


### Build Instructions:

Git Clone the repo and then run via visual studio
CMake to be added in the future

### Dependencies
- OpenGL
- GLFW
- GLAD
- GLM
- stb_image
- imGui 1.92.6

### Status
Under Development 1 / 12 finished


Be safe friend, don't you dare go hollow.