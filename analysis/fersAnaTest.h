//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Apr 24 13:57:42 2024 by ROOT version 6.30/04
// from TTree fersTree/fersData
// found on file: Run50.root
//////////////////////////////////////////////////////////

#ifndef fersAnaTest_h
#define fersAnaTest_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>

// Header file for the classes stored in the TTree if any.
#include "c++/11/vector"

class fersAnaTest {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          trigID;
   Float_t         trigTime;
   vector<unsigned short> *channelID;
   vector<unsigned short> *channelDataLG;
   vector<unsigned short> *channelDataHG;

   // List of branches
   TBranch        *b_trigID;   //!
   TBranch        *b_trigTime;   //!
   TBranch        *b_channelID;   //!
   TBranch        *b_channelDataLG;   //!
   TBranch        *b_channelDataHG;   //!

   fersAnaTest(TTree *tree=0);
   virtual ~fersAnaTest();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(TString outrootfilename);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef fersAnaTest_cxx
fersAnaTest::fersAnaTest(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   
   if (!tree) {
      Error("fersAnaTest", "No TTree provided and no default file path set.");
      return;
   }

   Init(tree);
}

fersAnaTest::~fersAnaTest()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t fersAnaTest::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t fersAnaTest::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void fersAnaTest::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   channelID = 0;
   channelDataLG = 0;
   channelDataHG = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("trigID", &trigID, &b_trigID);
   fChain->SetBranchAddress("trigTime", &trigTime, &b_trigTime);
   fChain->SetBranchAddress("channelID", &channelID, &b_channelID);
   fChain->SetBranchAddress("channelDataLG", &channelDataLG, &b_channelDataLG);
   fChain->SetBranchAddress("channelDataHG", &channelDataHG, &b_channelDataHG);
   Notify();
}

Bool_t fersAnaTest::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void fersAnaTest::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t fersAnaTest::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef fersAnaTest_cxx
