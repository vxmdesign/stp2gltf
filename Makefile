SRCS:= main.cpp LoadStep.cpp LoadFace.cpp Accessor.cpp
OBJS:= $(patsubst %.cpp, %.o, $(SRCS))

OCC_LIB = -lPTKernel \
	-lTKBin \
	-lTKBinL \
	-lTKBinTObj \
	-lTKBinXCAF \
	-lTKBO \
	-lTKBool \
	-lTKBRep \
	-lTKCAF \
	-lTKCDF \
	-lTKernel \
	-lTKFeat \
	-lTKFillet \
	-lTKG2d \
	-lTKG3d \
	-lTKGeomAlgo \
	-lTKGeomBase \
	-lTKHLR \
	-lTKIGES \
	-lTKLCAF \
	-lTKMath \
	-lTKMesh \
	-lTKMeshVS \
	-lTKNIS \
	-lTKOffset \
	-lTKPCAF \
	-lTKPLCAF \
	-lTKPrim \
	-lTKPShape \
	-lTKShapeSchema \
	-lTKShHealing \
	-lTKStdLSchema \
	-lTKStdSchema \
	-lTKSTEP209 \
	-lTKSTEP \
	-lTKSTEPAttr \
	-lTKSTEPBase \
	-lTKSTL \
	-lTKTObj \
	-lTKTopAlgo \
	-lTKVoxel \
	-lTKVRML \
	-lTKXCAF \
	-lTKXCAFSchema \
	-lTKXDEIGES \
	-lTKXDESTEP \
	-lTKXMesh \
	-lTKXml \
	-lTKXmlL \
	-lTKXmlTObj \
	-lTKXmlXCAF \
	-lTKXSBase \
	-lTKService \
	-lTKOpenGl \
	-lTKV3d




CFLAGS:= -g -I/usr/include/oce -I./microjson -L./microjson

all: stp2gltf

stp2gltf: microjson $(OBJS)
	g++ $(CFLAGS) -o $@ $(OBJS) $(OCC_LIB) -lmicrojson


microjson:
	git clone https://github.com/vxmdesign/microjson.git
	cd ./microjson && make static

%.o: %.cpp
	g++ $(CFLAGS) -c $^


clean:
	rm -f *.o
	rm -f stp2gltf

distclean: clean
	rm -rf microjson
