Windows, MSVC, x64. So the project is configured for this.

Launch through solution/project Visual Studio,
or with .exe with one directory with folders Shaders and Models (and manybe Textures if something from this folder is used)
Compiled .exe is already there in bin folder
assimp-vc142-mt.dll и zlib.dll must be within the same folder with .exe or same folder with solution/project

Dependencies: glew, GLFW, glm, SOIL, Assimp (Dependencies folder). There is what I use in the attached to github archive (should be dependencies folder).
Note that Assimp doesn't support 32bit.

Supported formats: in theory what Assimp supports.
In practice:
obj + mtl + textures - (also not all formats, for example progressive .jpg is not supported, see SOIL) - certainly yes.
Certainly not:
.blend, see github Assimp -> issues (github.com/assimp/assimp/issues/3316)
.c4d - no suitable reader

At the moment Textures folder contains textures for "manual", direct OpenGL use. Non-manual is via Assimp. 

Controls: mouse, mouse wheel changes FOV, WASD and L_CTRL/SPACE for camera movement. ESC to close the window. 
F - Wireframe rendering
