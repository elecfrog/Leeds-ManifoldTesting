# University of Leeds 2022-23
# COMP 5812M Assignment 1 Triangle Soup Renderer

This is a very simple renderer for triangle soup. It makes no attempt to be efficient, 
which keeps the code clearer.

To compile on the University Linux machines, you will need to do the following:

[userid@machine Assignment2MeshDSHandout]$ module add legacy-eng
[userid@machine Assignment2MeshDSHandout]$ module add qt/5.15.2
[userid@machine Assignment2MeshDSHandout]$ qmake -project QT+=opengl
[userid@machine Assignment2MeshDSHandout]$ qmake
[userid@machine Assignment2MeshDSHandout]$ make

You should see a compiler warning about an unused parameter, which can be ignored.

To execute the renderer, pass the file name on the command line:

[userid@machine Assignment2MeshDSHandout]$ ./Assignment2MeshDSHandout ../models/tetrahedron.tri
