This is a program to convert step files to gltf files.

This program is a quick and dirty tool I wrote to convert step to gltf. I created it for my own needs and I will update it as the need for new features crop up. I do not pretend that it captures every stp file correctly. I'm not sure it uses the opencascade libraries correctly. Their documentation is...challenging. I will not add a feature for you, but if you would like to contribute I'm down. 

This requires the liboce dev libraries for opencascade.
This also requires microjson which requires flex and bison. Microjson is automatically cloned and built as part of the build process

Outside of that it should have basically no other library requires and compiles under c++98


To build:
make

To run
stp2gltf stpfile gltffile binfile


stpfile is the step file input
gltffile is the gltf json file output
binfile is the binary file output




