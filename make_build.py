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


# copy ness-engine binaries
shutil.copy("vs-2010/Debug/ness_engine_d.dll", OUTDIR + "/lib/windows/x86/")
shutil.copy("vs-2010/Debug/ness_engine_d.lib", OUTDIR + "/lib/windows/x86/")
shutil.copy("vs-2010/Release/ness_engine.dll", OUTDIR + "/lib/windows/x86/")
shutil.copy("vs-2010/Release/ness_engine.lib", OUTDIR + "/lib/windows/x86/")


# copy new runtime to examples dir
if os.path.isdir("examples/ness-engine"):
    shutil.rmtree("examples/ness-engine")
shutil.copytree(OUTDIR, "examples/ness-engine")

raw_input("Done!")
