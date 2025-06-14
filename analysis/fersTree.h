#ifndef FERSTREE_H
#define FERSTREE_H

#include <TTree.h>
#include <vector>

using namespace std;

class fersTree {

 public:
  
  fersTree(TTree* t = 0);
  virtual ~fersTree() {};
  
  Int_t getEntry(Long64_t entry);

  void init(TTree* t);
  void reset();
  
  TTree* tree;
  
  UInt_t trigID;
  float trigTime;
  vector<UShort_t>*  channelID;
  vector<UShort_t >* channelDataHG;
  vector<UShort_t >* channelDataLG;

  TBranch* trigID_b;
  TBranch* trigTime_b;
  TBranch* channelID_b;
  TBranch* channelDataLG_b;
  TBranch* channelDataHG_b;
  
  ClassDef(fersTree, 1);
};

#endif
