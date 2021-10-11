import os
import re
import shutil
import subprocess


def read_lines(filename):
    with open(filename) as f:
        return [s.rstrip("\n") for s in f]


def write_lines(filename, lines):
    with open(filename, "w") as f:
        for s in lines:
            f.write(s + "\n")


# version

for s in read_lines("tee.cc"):
    m = re.match('#define version "(.+)"', s)
    if m:
        version = m[1]
if not version:
    print("tee.cc: version not defined")
    exit(1)


# build

subprocess.check_call("build-release.bat")

# zip

d = "tee-" + version
if os.path.exists(d):
    shutil.rmtree(d)
os.mkdir(d)

subprocess.check_call("copy *.exe " + d, shell=1)
subprocess.check_call("copy *.md " + d, shell=1)
subprocess.check_call("copy LICENSE " + d, shell=1)

subprocess.check_call("del *.zip", shell=1)
subprocess.check_call("7z a " + d + ".zip " + d)

shutil.rmtree(d)
