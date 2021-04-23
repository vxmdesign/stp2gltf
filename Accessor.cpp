#include "Accessor.h"

Accessor::Accessor(RGBColor *pRGB, Accessor *pParent){
  mFaces.clear();
  mRGB = pRGB;
  mParent = pParent;
}

void Accessor::addFace(LoadFace *pFace){
  unsigned int c;
  int offset;
  offset = 0;
  for(c = 0; c < mFaces.size(); c++){
    offset += mFaces[c]->vertexCount();
  }
  pFace->setOffset(offset);
  mFaces.push_back(pFace);  
}

Accessor *Accessor::parent(){
  return mParent;
}

int Accessor::vertexOffset(){
  int ret;
  if(mParent != NULL){
    ret = mParent->vertexOffset();
  }else{
    ret = 0;
  }
  ret+= (vertexCount() * sizeof(D3Point));
  return ret;
}

int Accessor::indicesOffset(){
  int ret;
  if(mParent != NULL){
    ret = mParent->indicesOffset();
  }else{
    ret = 0;
  }
  ret += (indicesCount() * sizeof(Indices));
  return ret;
}

void Accessor::addJson(JsonArray *pMesh, JsonArray *pAccessor, JsonArray *pMaterial){
  JsonObject *jo;
  JsonObject *jo2;
  JsonArray *ja;
  int sbo;
  int vbo;  
  D3Point vmax;
  D3Point vmin;
  int srt;
  int mtr;
  char buf[16];
  sbo = 0;
  vbo = 0;

  if(mParent != NULL){
    mParent->addJson(pMesh, pAccessor, pMaterial);
    sbo = mParent->indicesOffset();
    vbo = mParent->vertexOffset();
  }
  srt = pAccessor->count();
  mtr = pMaterial->count();
  if(mFaces.size() == 0){
    return;
  }
  sprintf(buf, "Mt%d", mtr);
  jo = new JsonObject(pMaterial);
  jo->addEntry("name", buf);
  pMaterial->addEntry(jo);
  jo2 = new JsonObject(jo);
  jo->addEntry("pbrMetallicRoughness", jo2);
  jo2->addEntry("metallicFactor", 0.0);
  ja = new JsonArray(jo2);
  jo2->addEntry("baseColorFactor", ja);
  ja->addEntry(new JsonNumber(mRGB->r));
  ja->addEntry(new JsonNumber(mRGB->g));
  ja->addEntry(new JsonNumber(mRGB->b));
  ja->addEntry(new JsonNumber(1.0));

  jo = new JsonObject(pAccessor);
  pAccessor->addEntry(jo);
  jo->addEntry("bufferView", 0);
  jo->addEntry("byteOffset", sbo);
  jo->addEntry("componentType", 5123);
  jo->addEntry("count", indicesCount() * 3);
  jo->addEntry("type", "SCALAR");
  ja = new JsonArray(ja);
  jo->addEntry("max", ja);
  ja->addEntry(new JsonNumber(imax()));
  ja = new JsonArray(ja);
  jo->addEntry("min", ja);
  ja->addEntry(new JsonNumber(imin()));

  vmax = getVmax();
  vmin = getVmin();
  jo = new JsonObject(pAccessor);
  pAccessor->addEntry(jo);
  jo->addEntry("bufferView", 1);
  jo->addEntry("byteOffset", vbo);
  jo->addEntry("componentType", 5126);
  jo->addEntry("count", vertexCount());
  jo->addEntry("type", "VEC3");
  ja = new JsonArray(ja);
  jo->addEntry("max", ja);
  ja->addEntry(new JsonNumber(vmax.x));
  ja->addEntry(new JsonNumber(vmax.y));
  ja->addEntry(new JsonNumber(vmax.z));
  ja = new JsonArray(ja);
  jo->addEntry("min", ja);
  ja->addEntry(new JsonNumber(vmin.x));
  ja->addEntry(new JsonNumber(vmin.y));
  ja->addEntry(new JsonNumber(vmin.z));

  
  jo = new JsonObject(pMesh);
  pMesh->addEntry(jo);
  jo->addEntry("mode", 4);
  jo->addEntry("material", mtr);
  jo->addEntry("indices", srt);
  jo2 = new JsonObject(jo);
  jo->addEntry("attributes", jo2);
  jo2->addEntry("POSITION", srt+1);  	       

}

void Accessor::addData(vector<D3Point> &pVertices, vector<Indices> &pIndices){
  unsigned int c;
  unsigned int d;
  D3Point vtx;
  Indices idx;
  if(mParent != NULL){
    mParent->addData(pVertices, pIndices);
  }
  if(mFaces.size() == 0){
    return;
  }
  for(c = 0; c < mFaces.size(); c++){
    for(d = 0; d < mFaces[c]->indexCount(); d++){
      idx = mFaces[c]->getIndices(d);
      pIndices.push_back(idx);
    }
    for(d = 0; d < mFaces[c]->vertexCount(); d++){
      vtx = mFaces[c]->getVertex(d);
      pVertices.push_back(vtx);
    }
  }
}

int Accessor::vertexCount(){
  int ret;
  unsigned int c;
  ret = 0;
  for(c = 0; c < mFaces.size(); c++){
    ret+= mFaces[c]->vertexCount();
  }
  return ret;
}

int Accessor::indicesCount(){
  int ret;
  unsigned int c;
  ret = 0;
  for(c = 0; c < mFaces.size(); c++){
    ret+= mFaces[c]->indexCount();
  }
  return ret;
}


unsigned short Accessor::imax(){
  unsigned int c;
  unsigned int d;
  unsigned short imax;
  Indices is;
  for(c = 0; c < mFaces.size(); c++){
    for(d = 0; d < mFaces[c]->indexCount(); d++){
      is = mFaces[c]->getIndices(d);
      if(c == 0 && d == 0){
	imax = is.a;
      }
      if(is.a > imax){
	imax = is.a;
      }
      if(is.b > imax){
	imax = is.b;
      }
      if(is.c > imax){
	imax = is.c;
      }
    }
  }
  return imax;
}

unsigned short Accessor::imin(){
  unsigned int c;
  unsigned int d;
  unsigned short imin;
  Indices is;
  for(c = 0; c < mFaces.size(); c++){
    for(d = 0; d < mFaces[c]->indexCount(); d++){
      is = mFaces[c]->getIndices(d);
      if(c == 0 && d == 0){
	imin = is.a;
      }
      if(is.a < imin){
	imin = is.a;
      }
      if(is.b < imin){
	imin = is.b;
      }
      if(is.c < imin){
	imin = is.c;
      }
    }
  }
  return imin;
}

D3Point Accessor::getVmax(){
  unsigned int c;
  unsigned int d;
  D3Point vmax;
  D3Point tmp;
  for(c = 0; c < mFaces.size(); c++){
    for(d = 0; d < mFaces[c]->vertexCount(); d++){
      tmp = mFaces[c]->getVertex(d);
      if(c == 0 && d == 0){
	vmax = tmp;
      }else{
	if(tmp.x > vmax.x){
	  vmax.x = tmp.x;
	}
	if(tmp.y > vmax.y){
	  vmax.y = tmp.y;
	}
	if(tmp.z > vmax.z){
	  vmax.z = tmp.z;
	}
      }
    }
  }
  vmax.x += 0.00001;
  vmax.y += 0.00001;
  vmax.z += 0.00001;
  
  return vmax;
}

D3Point Accessor::getVmin(){
  unsigned int c;
  unsigned int d;
  D3Point vmin;
  D3Point tmp;
  for(c = 0; c < mFaces.size(); c++){
    for(d = 0; d < mFaces[c]->vertexCount(); d++){
      tmp = mFaces[c]->getVertex(d);
      if(c == 0 && d == 0){
	vmin = tmp;
      }else{
	if(tmp.x < vmin.x){
	  vmin.x = tmp.x;
	}
	if(tmp.y < vmin.y){
	  vmin.y = tmp.y;
	}
	if(tmp.z < vmin.z){
	  vmin.z = tmp.z;
	}
      }
    }
  }
  vmin.x -= 0.00001;
  vmin.y -= 0.00001;
  vmin.z -= 0.00001;
  return vmin;
}

void Accessor::addFaceToList(LoadFace *pFace){
  if(pFace->getColor() == mRGB){
    addFace(pFace);
    return;
  }
  if(mParent != NULL){
    mParent->addFaceToList(pFace);
    return;
  }
  printf("couldn't add face\n");
}

void Accessor::clearParent(){
  mParent = NULL;
}

void Accessor::debugPrint(){
  printf("C: %f %f %f: %d\n", mRGB->r, mRGB->g, mRGB->b, (int)mFaces.size());
  if(mParent != NULL){
    mParent->debugPrint();
  }
}
