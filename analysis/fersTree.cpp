#include "fersTree.h"

ClassImp(fersTree)

fersTree::fersTree(TTree* t) : tree(0) {

  trigID     = 0;
  trigTime     = 0;
  channelID    = new vector<UShort_t>();
  channelDataLG  = new vector<UShort_t>();
  channelDataHG  = new vector<UShort_t>();

  if (t) init(t);
}

Int_t fersTree::getEntry(Long64_t entry) {  
  if (!tree)                      return 0;
  if (entry > tree->GetEntries()) return 0;  
  return tree->GetEntry(entry);
}

void fersTree::init(TTree *t) {

  if (!t) return;
  
  tree = t;

  trigID_b = tree->GetBranch("trigID");
  if(!trigID_b)
    trigID_b = tree->Branch("trigID", &trigID);
  else
    trigID_b->SetAddress(&trigID);

  trigTime_b = tree->GetBranch("trigTime");
  if(!trigTime_b)
    trigTime_b = tree->Branch("trigTime", &trigTime);
  else
    trigTime_b->SetAddress(&trigTime);
  
  channelID_b = tree->GetBranch("channelID");
  if (!channelID_b)
    channelID_b = tree->Branch("channelID", &channelID);
  else
    channelID_b->SetAddress(&channelID);

  channelDataLG_b = tree->GetBranch("channelDataLG");
  if (!channelDataLG_b)
    channelDataLG_b = tree->Branch("channelDataLG", &channelDataLG);
  else
    channelDataLG_b->SetAddress(&channelDataLG);

  channelDataHG_b = tree->GetBranch("channelDataHG");
  if (!channelDataHG_b)
    channelDataHG_b = tree->Branch("channelDataHG", &channelDataHG);
  else
    channelDataHG_b->SetAddress(&channelDataHG);

  reset();
}

void fersTree::reset() {
  trigID = 0; 
  trigTime = 0; 
  channelID->clear();
  channelDataLG->clear();
  channelDataHG->clear();
}
