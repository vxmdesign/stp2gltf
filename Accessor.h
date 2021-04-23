#ifndef ACCESSOR_H
#define ACCESSOR_H

#include <stdio.h>
#include <vector>
#include "LoadFace.h"
#include "uJson.h"

using namespace std;

class Accessor{
 public:
  Accessor(RGBColor *pRGB, Accessor *pParent);
  void addFace(LoadFace *pFace);
  void addFaceToList(LoadFace *pFace);
  Accessor *parent();
  int vertexOffset();
  int indicesOffset();
  void addJson(JsonArray *pMesh, JsonArray *pAccessor, JsonArray *pMaterial);
  void addData(vector<D3Point> &pVertices, vector<Indices> &pIndices);

  void debugPrint();
  void clearParent(); //delete eventually

 private:
  int vertexCount();
  int indicesCount();
  unsigned short imax();
  unsigned short imin();
  D3Point getVmax();
  D3Point getVmin();

  vector<LoadFace *> mFaces;
  RGBColor *mRGB;
  Accessor *mParent;
};

#endif
