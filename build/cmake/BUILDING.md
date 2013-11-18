# Building with CMake #

## Requirements ##

The following must be available on the system:

1. GLEW
2. OpenCTM
3. FreeGLUT
4. glm

Linux: Packages for glm, GLEW and FreeGLUT are available for most major distributions. It may be necessary to download and build OpenCTM from [source (available here)](sourceforge.net/projects/openctm/). For users of Arch Linux, an AUR package is [available here](https://aur.archlinux.org/packages/openctm/).

SOIL and glsw will be built from source:

* Please download glsw from [this page (bottom link)](http://prideout.net/blog/?p=11) and place the 4 source files in "/lib/glsw".

* Please download SOIL from [this page](http://www.lonesock.net/soil.html), unzip and place the contents in "/lib/SOIL" (The src/ and lib/ directories should end up directly within SOIL/).

## Building ##

On Linux platforms, the following commands should suffice (starting at repository directory):

$ cd build/cmake
$ cmake ../..
$ make
