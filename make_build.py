import os, shutil

BUILD_TYPE = 'x86'
PLATFORM = 'windows'

# create/recreate the last_build dir
if os.path.isdir("last_build"):
    os.rmdir("last_build")
os.mkdir("last_build")
os.mkdir("last_build/lib")



raw_input("Done!")
