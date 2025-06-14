#ifndef ROOT_FERSREAD
#define ROOT_FERSREAD

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "TFile.h"
#include "TTree.h"
#include "fersTree.h"

using namespace std;

class fersRead {	
 
 public:
  
  fersRead(string ifilename = "", string ofilename = "", string gain = "LG");
  virtual ~fersRead() {}
  
  void read(); // Read all events, store them in a TTree and save them to file
  
  void setGain (string gain) { gain_ = gain; }
  void setInputFile (string ifilename) { ifilename_ = ifilename; }
  void setOutputFile(string ofilename) { ofilename_ = ofilename; }  
  void setDebugger  (bool   debugger)  { debugger_  = debugger; }
  
 private:
  
  fersTree t; // Container class for event variables
  
  string ifilename_;
  string ofilename_;
  string gain_;
  string otreename_;
  TTree* otree_;

  bool debugger_;

  size_t eventCounter_;
  size_t nLineRead_;


  ClassDef(fersRead, 1);
};

#endif
