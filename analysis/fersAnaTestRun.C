// analysis file for experimental comparison with FERS
// written by Matteo Negrini
// also the other files starting with "fers"


#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include "fersAnaTest.C"

void fersAnaTestRun(TString filename, TString outfilename) {

  TTree *tree;
  TFile f(filename);
  f.GetObject("fersTree",tree);
  
  fersAnaTest a(tree);
  a.Loop(outfilename);
  
  delete tree;

}

