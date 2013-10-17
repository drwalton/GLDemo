# GLDemo #

This tiny library contains some helper classes for building tiny OpenGL tech demos with FreeGLUT, GLM and OpenCTM. It is currently a work in progress.
 
## Dependencies ##

    freeGLUT: http://sourceforge.net/projects/freeglut/
    glew: http://sourceforge.net/projects/glew/
    glm: http://sourceforge.net/projects/ogl-math/
    openCTM: http://sourceforge.net/projects/openctm/
    glsw: http://prideout.net/blog/?p=11

All are released under the MIT License, except openCTM, which is released under the (similar) zlib/libpng license.

## Examples ##

Included in this repository are the following example projects:

### Silhouette ###

This is a demo of a simple way to render silhouettes in modern OpenGL.

The basic idea is to render the back of the object (expanded in some way) in black, and then render the front as usual over the top.

The demo shows a few different ways the backfaces can be expanded, and also includes a very basic Gooch cel shader for the frontfaces.
