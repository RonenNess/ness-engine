ness-engine
===========

NessEngine is a 2D rendering engine for games.

## folders in this git

distribution: 
required libs and resources for the building process. this folder is used by the make_build.py script.

sdl: 
required sdl include files for the building process. this folder is used by the make_build.py script.

source: 
ness-engine source files.

vs-2010: 
visual studio 2010 project to build the engine for x86.

vs-2013: 
visual studio 2013 project to build the engine for x64 or x86.

builds:
all the finished builds, ready to be used.

examples:
set of example projects using ness-engine.



## building ness-engine
To build a ness-engine package follow these steps:

1. compile with all projects and platforms:
	1.a. vs2010 x86 (release + debug)
	1.b. vs2013 x86 (release + debug)
	1.c. vs2013 x64 (release + debug)
2. run make_build.py script
3. a new folder will be created: last_build. inside that folder you will find 'ness-engine' folder, which will contain the ready build.
4. zip it, and don't forget to increment Ness-Engine version (in NessEngine.h macro called NESS_VERSION and NESS_SUB_VERSION)