#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

#include "Accessor.h"
#include "LoadStep.h"
#include "LoadFace.h"
#include "uJson.h"

using namespace std;
JsonRoot *getEmptyJson(){
  string json;
  char gltfbase[] =
    "{\"asset\":{\"generator\":\"STP2GLTF\",\"version\":\"2.0\"},"
    "\"scene\":0,"
    "\"scenes\":[{\"nodes\":[0]}],"
    "\"nodes\":[{\"children\":[1]},{\"mesh\":0}],"
    "\"meshes\":[{\"primitives\":[],\"name\":\"Mesh\"}],"
    "\"accessors\":[],"
    "\"materials\":[],"
    "\"bufferViews\":[],"
    "\"buffers\":[]}";
  json = string(gltfbase);
  JSON::JsonDriver jd;
  return jd.parse(json);    
}


void loadGltf(Accessor *pAcc, const char *pGltfFile, const char *pBinFile){
  JsonRoot *jr;
  JsonObject *jo;
  JsonObject *jo2;
  JsonArray *ja;
  JsonArray *mesh, *acc, *mtr;
  string json;
  vector<D3Point> vtx;
  vector<Indices> idc;
  D3Point vp;
  Indices ip;
  int isz;
  int vsz;
  int fd;
  unsigned int c;
  jr = getEmptyJson();
  if(CHECK_ASSIGN_OBJECT(jo, jr->getNode()) < 0){
    printf("base failed to load\n");
    return;
  }
  if(CHECK_ASSIGN_ARRAY(mesh, jo->get("meshes")) < 0){
    printf("base mesh failed to load\n");
    return;
  }
  if(CHECK_ASSIGN_OBJECT(jo2, mesh->get(0)) < 0){
    printf("error 1\n");
    return;
  }  
  if(CHECK_ASSIGN_ARRAY(mesh, jo2->get("primitives")) < 0){
    printf("base primitives failed to load\n");
    return;
  }
  if(CHECK_ASSIGN_ARRAY(acc, jo->get("accessors")) < 0){
    printf("base accesors failed to load\n");
    return;
  }
  if(CHECK_ASSIGN_ARRAY(mtr, jo->get("materials")) < 0){
    printf("base materials failed to load\n");
  }
  pAcc->addJson(mesh, acc, mtr);
  vtx.clear();
  idc.clear();
  pAcc->addData(vtx, idc);
  isz = idc.size() * sizeof(Indices);
  vsz = vtx.size() * sizeof(D3Point);
  if(CHECK_ASSIGN_ARRAY(ja, jo->get("bufferViews")) < 0){
    printf("no buffer view\n");
    return;
  }
  
  jo2 = new JsonObject(ja);
  ja->addEntry(jo2);
  jo2->addEntry("buffer", 0);
  jo2->addEntry("byteOffset", 0);
  jo2->addEntry("byteLength", isz);
  
  jo2 = new JsonObject(ja);
  ja->addEntry(jo2);  
  jo2->addEntry("buffer", 0);
  jo2->addEntry("byteOffset", isz);
  jo2->addEntry("byteLength", vsz);
  jo2->addEntry("byteStride", 12);

  if(CHECK_ASSIGN_ARRAY(ja, jo->get("buffers")) < 0){
    printf("no buffer view\n");
    return;
  }
  jo2 = new JsonObject(ja);
  ja->addEntry(jo2);
  jo2->addEntry("byteLength", isz + vsz);
  jo2->addEntry("uri", pBinFile);

  fd = open(pGltfFile, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
  if(fd < 0){
    return;
  }  
  json = jr->serialize();
  if(write(fd, json.c_str(), json.length()) != json.length()){
    printf("failed to write gltf\n");        
  }
  close(fd);

  fd = open(pBinFile, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
  if(fd < 0){
    return;
  }  

  for(c = 0; c < idc.size(); c++){
    ip = idc[c];
    if(write(fd, (unsigned char*)&ip, sizeof(ip))!= sizeof(ip)){
      printf("failed during idc write\n");
      close(fd);
      return;      	
    }
  }
  for(c = 0; c < vtx.size(); c++){
    vp = vtx[c];
    if(write(fd, (unsigned char*)&vp, sizeof(vp))!= sizeof(vp)){
      printf("failed during vtx write\n");
      close(fd);
      return;      	
    }
  }
  close(fd);  
}

int main(int argc, char **argv){
  LoadStep *ls;
  Accessor *acc;
  if(argc != 4){
    printf("Usage: stp2gltf <stpfile> <gltf file> <bin file>\n");
  }
  ls = new LoadStep(argv[1]);
  acc = ls->getAccessors();
  loadGltf(acc, argv[2], argv[3]);  
}
