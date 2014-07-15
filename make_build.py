import os, shutil
OUTDIR = "last_build/ness-engine"

# if already exist previous build, remove it
if os.path.isdir(OUTDIR):
    shutil.rmtree(OUTDIR)

# copy the libs dir based on platform and build type, and also copy the readme files
shutil.copytree("distribution", OUTDIR)

# copy the include files
shutil.copytree("sdl/include", OUTDIR + "/include")

# copy all the ness-engine header files and directories structure
for root, dirs, files in os.walk("source"):

    # remove the 'source/NessEngine' part from the dir
    relative_path = root[len("source/NessEngine"):]

    # if directory does not exist in destination, create it
    FullDirPath = OUTDIR + "/include" + relative_path
    if not os.path.exists(FullDirPath):
        os.makedirs(FullDirPath)

    # now loop over all files in dir and copy all header files
    for file in files:
        if file[-2:] != '.h':
            continue
        src = os.path.join(root, file)
        dest = os.path.join(FullDirPath, file)
        shutil.copyfile(src, dest)


# copy ness-engine windows binaries
for version in ["vs2010", "vs2013"]:
    LibDir = OUTDIR + "/lib/win_x86/" + version + "/"
    os.mkdir(LibDir)
    shutil.copy(version + "/Debug/ness_engine_d.dll", LibDir)
    shutil.copy(version + "/Debug/ness_engine_d.lib", LibDir)
    shutil.copy(version + "/Release/ness_engine.dll", LibDir)
    shutil.copy(version + "/Release/ness_engine.lib", LibDir)


# copy new runtime to examples dir
if os.path.isdir("examples/ness-engine"):
    shutil.rmtree("examples/ness-engine")
shutil.copytree(OUTDIR, "examples/ness-engine")
shutil.copy("examples/ness-engine/lib/win_x86/vs2010/ness_engine.dll", "examples/ness-engine/lib/win_x86/")
shutil.copy("examples/ness-engine/lib/win_x86/vs2010/ness_engine.lib", "examples/ness-engine/lib/win_x86/")
shutil.copy("examples/ness-engine/lib/win_x86/vs2010/ness_engine_d.dll", "examples/ness-engine/lib/win_x86/")
shutil.copy("examples/ness-engine/lib/win_x86/vs2010/ness_engine_d.lib", "examples/ness-engine/lib/win_x86/")

raw_input("Done!")
