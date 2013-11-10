CS 174A: Introduction to Computer Graphics: Assignment 3
Weight: 15 %
Maximum points: 40
Collaboration: None. If you discuss this assignment with others you should submit their names along with the assignment
material.
Start working on this assignment early. You will not have time to do it well at the last minute.
Instructions: Submit your assignment on CourseWeb in an archive named <surname>.zip, where <surname> denotes
your last name. This should include ALL of the code necessary to compile and run the program, and should not contain any
functionality beyond what is requested below. The main function should be in a file named raytrace.cpp, and your
executable should be named raytrace.exe .
Your solution should not contain any OpenGL API calls. However, you may use OpenGL to display your results during
debugging. In the Materials/Homework section of the course website, you will find two pieces of code: one inverts a 4x4
matrix, and the other writes a char buffer to a ppm image, which is the expected output of this program.
For this assignment, you will be implementing a Ray Tracer. Chapter 11.1-3 of Edward Angel, Interactive Computer
Graphics, 6th Edition, outlines what is involved in this process. Your system need only handle the rendering of spheres,
with a camera situated at the origin of a right-handed coordinate system, looking down the negative z axis. Local
illumination, reflections, and shadows must be implemented. The following will be taken as inputs:
a. The near plane2, left2, right2, top2, and bottom2
b. The resolution of the image nColumns1 x nRows1
c. The position2 and scaling2 (non-uniform), color3, Ka
3, Kd
3
, Ks
3, Kr
3 and the specular exponent n2 for up to five
spheres
d. The position2 and intensity2 for up to five positional, point light sources
e. The background color2
f. The scene’s ambient intensity2
g. The output file name
1 int 2 float 3 float in the range [0..1]
Your program should take a single filename as a command-line argument. The input file describes the scene in the
following format:
NEAR <n>
LEFT <l>
RIGHT <r>
BOTTOM <b>
TOP <t>
RES <x> <y>
SPHERE <name> <pos x> <pos y> <pos z> <scl x> <scl y> <scl z> <r> <g> <b> <Ka> <Kd> <Ks> <Kr> <n>
LIGHT <name> <pos x> <pos y> <pos z> <Ir> <Ig> <Ib>
BACK <r> <g > <b>
AMBIENT <Ir> <Ig> <Ib>
OUTPUT <name>
All fields (NEAR, LEFT, RIGHT, etc.) are separated by one or more carriage returns. Note that the SPHERE and LIGHT
fields may appear up to five times each. Also, the fields may appear in any order, and blank lines may occur between them.
All parameters (<name>, <posx>, <posy>, etc.) are separated by one or more white space characters such as spaces or tabs.
There will be no angle brackets in the input file; the ones above merely indicate the parameters. Also, names should be
limited to 20 characters with no spaces.
(cont’d)