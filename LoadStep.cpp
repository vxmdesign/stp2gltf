#include "LoadStep.h"

LoadStep::LoadStep(const char *pFilename){
  STEPCAFControl_Reader sreader;
  TDF_LabelSequence root;
  TDF_Label lb;
  TopoDS_Shape shape;
  TopoDS_Face face;
  int i;
  int hash;
  TopExp_Explorer ex;
  LoadFace *lf;
  RGBColor *rgb;  
  mFaces.clear();
  mColors.clear();
  mColorMap.clear();
  sreader.ReadFile(pFilename);
  sreader.SetColorMode(true);
  sreader.SetNameMode(true);
  sreader.SetLayerMode(true);
  mDoc = new TDocStd_Document("StepReader"); 
  sreader.Transfer(mDoc);
  mShapeTool = XCAFDoc_DocumentTool::ShapeTool(mDoc->Main());
  mShapeTool->GetShapes(root);
  loadColors(sreader.Reader().WS());    
  rgb = (RGBColor*)malloc(sizeof(RGBColor));
  rgb->r = 0.5;
  rgb->g = 0.5;
  rgb->b = 0.5;
  mColors.push_back(rgb);
  printf("Found %d Colors\n", (int)mColors.size());
  for(i = 0; i < sreader.Reader().NbShapes(); i++){
    shape = sreader.Reader().Shape(i+1);
    for(ex.Init(shape, TopAbs_FACE); ex.More(); ex.Next()){
      face = TopoDS::Face(ex.Current());
      lb = mShapeTool->FindShape(face, Standard_False);
      //if(!lb.IsNull()){
      //printf("no style shape %d\n", lb.Tag());
      //	cout<<lb<<endl;
      //}
      lf = new LoadFace(face);
      if(!lb.IsNull()){
	hash = TDF_LabelMapHasher::HashCode(lb, INT_MAX);
      }
      if(!lb.IsNull() && mColorMap.find(hash) != mColorMap.end()){
	lf->setColor(mColorMap[hash]);
      }else{
	lf->setColor(rgb);
      }
      mFaces.push_back(lf);
    }
  }  
  printf("total colors %d\n",(int)mColors.size());
  printf("Total faces: %d\n",(int)mFaces.size());
	 
}

void LoadStep::loadColors(Handle(XSControl_WorkSession) pWS){
  STEPConstruct_Styles Styles(pWS);
  Handle(StepVisual_StyledItem) style;
  Handle(StepVisual_Colour) surf, bound, curv;
  Handle(TColStd_HSequenceOfTransient) invis;
  Standard_Boolean comp = false;
  int i;
  bool hascolor;
  Quantity_Color col;
  RGBColor *rgb;
  TopoDS_Shape shape;
  unsigned int c;
  bool invisible;
  TDF_Label lb;

  LoadFace *lf;
  TopExp_Explorer ex;  
  TopoDS_Face face;

	    
  invis = new TColStd_HSequenceOfTransient;  
  if(!Styles.LoadStyles()){
    printf("No styles\n");
    return;
  }
  Styles.LoadInvisStyles(invis);
  for(i = 0; i < Styles.NbStyles(); i++){
    style = Styles.Style(i+1);
    invisible = false;
    for(c = 0; c < invis->Length() && invisible == false; c++){
      if(style == invis->Value(c)){
	printf("invisible item\n");
	invisible = true;
      }
    }
    
    shape = STEPConstruct::FindShape(Styles.TransientProcess(), style->Item());
    hascolor = false;
    if(!style.IsNull() && invisible == false){
      if(Styles.GetColors(style, surf, bound, curv, comp)){
	if(!surf.IsNull()){
	  Styles.DecodeColor(surf,col);
	  hascolor = true;
	}else if(!bound.IsNull()){
	  Styles.DecodeColor(bound,col);
	  hascolor = true;
	}else if(!curv.IsNull()){
	  Styles.DecodeColor(curv,col);
	  hascolor = true;
	}
	if(hascolor == true){
	  rgb = NULL;
	  
	  for(c = 0; c < mColors.size() && rgb == NULL; c++){
	    if(mColors[c]->r == col.Red() &&
	       mColors[c]->g == col.Green() &&
	       mColors[c]->b == col.Blue()){
	      rgb = mColors[c];	    
	    }
	  }
	  if(rgb == NULL){
	    rgb = (RGBColor*)malloc(sizeof(RGBColor));
	    rgb->r = col.Red();
	    rgb->g = col.Green();
	    rgb->b = col.Blue();
	    mColors.push_back(rgb);
	  }
	  lb = mShapeTool->FindShape(shape, Standard_True);
	  //if(!lb.IsNull()){
	  //  printf("style shape %d\n", lb.Tag());
	  //  cout<<lb<<endl;
	  //}
	  //printf("Color: %f %f %f\n", rgb->r, rgb->g, rgb->b);
	  if(!lb.IsNull()){
	    mColorMap[TDF_LabelMapHasher::HashCode(lb,INT_MAX)] = rgb;
	  }
	}
	
      }	 
    }
  }
}

LoadFace *LoadStep::getFace(int pIndex){
  if(pIndex >= mFaces.size()){
    return NULL;
  }
  return mFaces[pIndex];
}

unsigned int LoadStep::count(){
  return mFaces.size();
}

Accessor *LoadStep::getAccessors(){
  unsigned int c;
  Accessor *top;
  top =NULL;
  for(c = 0; c < mColors.size(); c++){
    top = new Accessor(mColors[c], top);
  }
  for(c = 0; c < mFaces.size(); c++){
    top->addFaceToList(mFaces[c]);
  }
  top->debugPrint();
  return top;
}

Accessor *LoadStep::getAccessors(int pDbgIdx){
  int idx;
  unsigned int c;
  Accessor *top;
  Accessor *ret;
  top =NULL;
  ret =NULL;
  for(c = 0; c < mColors.size(); c++){
    top = new Accessor(mColors[c], top);
    if(c == pDbgIdx){
      ret = top;
    }
  }
  for(c = 0; c < mFaces.size(); c++){
    top->addFaceToList(mFaces[c]);
  }
  top->debugPrint();
  if(ret != NULL){    
    ret->clearParent();
    return ret;
  }
  return top;
}


void LoadStep::printLabel(TDF_Label pLabel){
  cout<<pLabel<<endl;
  if(pLabel.HasChild()){
    for(TDF_ChildIterator it1(pLabel,Standard_False); it1.More(); it1.Next()){
      printLabel(it1.Value());
    }    
  }
}
