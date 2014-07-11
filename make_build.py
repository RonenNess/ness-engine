import os, shutil

BUILD_TYPE = 'x86'
PLATFORM = 'windows'

# if already exist previous build, remove it
if os.path.isdir("last_build"):
    shutil.rmtree("last_build")

# create the last-build dir
os.mkdir("last_build")

# copy the libs dir based on platform and build type, and also copy the readme files
shutil.copytree(os.path.join("libs", PLATFORM, BUILD_TYPE), "last_build/lib")
shutil.copyfile(os.path.join("libs", "README-NESSENGINE.txt"), "last_build/lib/README-NESSENGINE.txt")
shutil.copyfile(os.path.join("libs", "README-SDL.txt"), "last_build/lib/README-SDL.txt")

# copy the include files
shutil.copytree("sdl/include", "last_build/include")

raw_input("Done!")
