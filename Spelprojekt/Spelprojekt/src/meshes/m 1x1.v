v -0.5 0.5 0.5
v -0.5 -0.5 0.5
v 0.5 0.5 0.5
v 0.5 -0.5 0.5
v -0.5 0.5 -0.5
v -0.5 -0.5 -0.5
v 0.5 0.5 -0.5
v 0.5 -0.5 -0.5
vt 0.0 1.0
vt 0.0 0.0
vt 1.0 1.0
vt 1.0 0.0
#vert1 uv1 norm1 | vert2 uv2 norm2 | vert3 uv3 norm3 
#Near
f 1 1 0 2 1 0 3 1 0
f 3 3 0 2 1 0 4 1 0
#Top
f 5 1 0 1 1 0 7 1 0
f 7 1 0 1 1 0 3 1 0
#Right
f 3 1 0 4 1 0 7 1 0
f 7 1 0 4 1 0 8 1 0
#Left
f 5 1 0 6 1 0 1 1 0
f 1 1 0 6 1 0 2 1 0
#Botom
f 2 1 0 6 1 0 4 1 0
f 4 1 0 6 1 0 8 1 0
#Behind
f 7 1 0 8 1 0 5 1 0
f 5 1 0 8 1 0 6 1 0