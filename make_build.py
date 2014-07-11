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
shutil.copyfile(os.path.join("libs", PLATFORM, "README.txt"), "last_build/README.txt")

# copy the include files
shutil.copytree("sdl/include", "last_build/include")

# copy all the ness-engine header files and directories structure
for root, dirs, files in os.walk("source"):

    # remove the 'source/NessEngine' part from the dir
    relative_path = root[len("source/NessEngine"):]

    # if directory does not exist in destination, create it
    FullDirPath = "last_build/include" + relative_path
    if not os.path.exists(FullDirPath):
        os.makedirs(FullDirPath)

    # now loop over all files in dir and copy all header files
    for file in files:
        if file[-2:] != '.h':
            continue
        src = os.path.join(root, file)
        dest = os.path.join(FullDirPath, file)
        shutil.copyfile(src, dest)

raw_input("Done!")
