#include "LoadFace.h"

LoadFace::LoadFace(TopoDS_Face &pFace){
  mVertices.clear();
  mIndices.clear();
  mRGB = NULL;
  int i;
  D3Point pt;
  Indices is;
  int a,b,c;
  mOffset = 0;
  mHash = pFace.HashCode(INT_MAX);
  Standard_Boolean reversed = (pFace.Orientation() == TopAbs_REVERSED);
  BRepMesh_IncrementalMesh mesh(pFace, 0.1);
  TopLoc_Location loc = pFace.Location();
  Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(pFace, loc);
  gp_Trsf transform = loc;
  const TColgp_Array1OfPnt& arrPolyNodes = triangulation->Nodes();
  const Poly_Array1OfTriangle& arrTriangles = triangulation->Triangles();
  gp_Pnt transformedPoint;
  for(i = 0; i < triangulation->NbNodes();i++){
    transformedPoint = arrPolyNodes(i+1).Transformed(transform);
    pt.x = transformedPoint.X();
    pt.y = transformedPoint.Y();
    pt.z = transformedPoint.Z();
    //gp_XYZ v(arrPolyNodes(i+1).Coord());
    //pt.x = v.X();
    //pt.y = v.Y();
    //pt.z = v.Z();
    mVertices.push_back(pt);
  }
  for(i = 0; i < triangulation->NbTriangles(); i++){
    arrTriangles(i+1).Get(a,b,c);
    is.a = a - 1;
    if(reversed){
      is.c = b - 1;
      is.b = c - 1;
    }else{
      is.b = b - 1;
      is.c = c - 1;
    }
    mIndices.push_back(is);
  }
}

D3Point LoadFace::getVertex(int pIndex){
  D3Point ret;
  ret.x = -1;
  ret.y = -1;
  ret.z = -1;
  if(pIndex >= mVertices.size()){
    return ret;
  }
  return mVertices[pIndex];
}

Indices LoadFace::getIndices(int pIndex){
  Indices ret;
  ret.a = -1;
  ret.b = -1;
  ret.c = -1;  
  if(pIndex >= mIndices.size()){
    return ret;
  }
  ret = mIndices[pIndex];
  ret.a += mOffset;
  ret.b += mOffset;
  ret.c += mOffset;
  return ret;
}

unsigned int LoadFace::vertexCount(){
  return mVertices.size();
}

unsigned int LoadFace::indexCount(){
  return mIndices.size();
}

int LoadFace::hash(){
  return mHash;
}


void LoadFace::setColor(RGBColor *pRGB){
  mRGB = pRGB;  
}

RGBColor *LoadFace::getColor(){
  return mRGB;
}


void LoadFace::setOffset(int pOffset){
  mOffset = pOffset;
}

void LoadFace::printFace(){
  unsigned int c;
  D3Point pt;
  Indices i;
  printf("Face: I %d V %d\n", (int)mIndices.size(), (int)mVertices.size());
	
  for(c = 0; c < mVertices.size(); c++){
    pt = mVertices[c];
    printf("V: %f %f %f\n", pt.x,pt.y,pt.z);
  }
  for(c = 0; c < mIndices.size(); c++){
    i = mIndices[c];
    printf("I: %d %d %d\n", i.a, i.b, i.c);
    
  }
}
