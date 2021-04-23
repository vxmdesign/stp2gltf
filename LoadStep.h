#ifndef LOAD_STEP_H
#define LOAD_STEP_H

#include <stdio.h>
#include <vector>
#include <map>
#include <malloc.h>
#include <TDocStd_Document.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDF_ChildIterator.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPConstruct_Styles.hxx>
#include <STEPConstruct.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>
#include <TopExp_Explorer.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include <Quantity_Color.hxx>
#include <StepVisual_StyledItem.hxx>
#include <TDF_LabelMapHasher.hxx>

#include "LoadFace.h"
#include "Accessor.h"
using namespace std;

class LoadStep{
 public:
  LoadStep(const char *pFilename);
  LoadFace *getFace(int pIndex);
  unsigned int count();
  Accessor *getAccessors();
  Accessor *getAccessors(int pDbgIdx);
 private:
  void printLabel(TDF_Label pLabel);
  void loadColors(Handle(XSControl_WorkSession) pWS);
  Handle(TDocStd_Document) mDoc;
  Handle(XCAFDoc_ShapeTool) mShapeTool;
  vector<LoadFace*> mFaces;
  vector<RGBColor*> mColors;
  map<int, RGBColor*> mColorMap;
};


#endif
