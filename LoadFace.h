#ifndef LOAD_FACE_H
#define LOAD_FACE_H

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <BRep_Tool.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Poly_Triangulation.hxx>
#include <Poly_PolygonOnTriangulation.hxx>

typedef struct{
  float x;
  float y;
  float z;
}D3Point;

typedef struct{
  unsigned short a;
  unsigned short b;
  unsigned short c;
}Indices;

typedef struct{
  float r;
  float g;
  float b;
}RGBColor;

#define TRIANGULATION_SCALE 0.1


using namespace std;

class LoadFace{
 public:
  LoadFace(TopoDS_Face &pFace);
  D3Point getVertex(int pIndex);
  Indices getIndices(int pIndex);
  unsigned int vertexCount();
  unsigned int indexCount();
  int hash();
  void setColor(RGBColor *pRGB);
  RGBColor *getColor();
  void setOffset(int pOffset);
  void printFace();
 private:
  RGBColor *mRGB;
  vector<D3Point> mVertices;
  vector<Indices> mIndices;
  int mHash;
  int mOffset;
};

#endif
