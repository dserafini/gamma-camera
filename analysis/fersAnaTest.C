#define fersAnaTest_cxx
#include "fersAnaTest.h"
#include <TH2.h>
#include <TTree.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <sstream>


void fersAnaTest::Loop(TString outrootfilename="outputHistos.root")
{
//   In a ROOT session, you can do:
//      root> .L fersAnaTest.C
//      root> fersAnaTest t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   
   TFile* outfile = TFile::Open(outrootfilename, "RECREATE");   

  // valori di X  dei canali 0-63 
  int const chID2mapX[64] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 
                                0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7};
  
  //Valori di Y tra i canali 0-63 
  int const chID2mapY[64] = {6, 5, 7, 4, 7, 4, 6, 5, 6, 5, 7, 4, 7, 4, 6, 5, 6, 5, 7, 4, 7, 4, 6, 5, 6, 5, 7, 4, 7, 4, 6, 5, 
  				2, 1, 3, 0, 3, 0, 2, 1, 2, 1, 3, 0, 3, 0, 2, 1, 2, 1, 3, 0, 3, 0, 2, 1, 2, 1, 3, 0, 3, 0, 2, 1};
  
  double const pix_size = 3.2; // mm
  
  TH1D * ampliDistLG[64];
  TH1D * ampliDistHG[64];
  for (int i = 0; i<64; ++i){
    std::ostringstream hnameL, hnameH;
    hnameL<<"channel_"<<i<<"_LG_Distribution";
    ampliDistLG[i] = new TH1D(hnameL.str().c_str(),hnameL.str().c_str(), 4096, -0.5, 4095.5);
    hnameH<<"channel_"<<i<<"_HG_Distribution";
    ampliDistHG[i] = new TH1D(hnameH.str().c_str(),hnameH.str().c_str(), 4096, -0.5, 4095.5);
  }
   
   TH1D* TotalCharge =  new TH1D("TotalCharge","TotalCharge", 2001, -5, 20005);
   TH1D* MainClustCharge =  new TH1D("MainClustCharge","MainCLustCharge", 2001, -5, 20005);
   TH1D* MainPixCharge =  new TH1D("MainPixCharge","MainPixCharge", 2001, -5, 20005);

   const int nbins = 16;
   TH1D* xglob =  new TH1D("xglob","xglob", nbins, -12.8, 12.8);
   TH1D* yglob =  new TH1D("yglob","yglob", nbins, -12.8, 12.8);
   TH1D* xclus =  new TH1D("xclus","xclus", nbins, -12.8, 12.8);
   TH1D* yclus =  new TH1D("yclus","yclus", nbins, -12.8, 12.8);
   TH2D* xyglob =  new TH2D("xyglob","xyglob", 320, -12.8, 12.8, 320, -12.8, 12.8);
   TH2D* xyclus =  new TH2D("xyclus","xyclus", 320, -12.8, 12.8, 320, -12.8, 12.8);
   TH2D* xyglobloose =  new TH2D("xyglobloose","xyglobloose", nbins, -12.8, 12.8, nbins, -12.8, 12.8);
   TH2D* xyclusloose =  new TH2D("xyclusloose","xyclusloose", nbins, -12.8, 12.8, nbins, -12.8, 12.8);

   TH2I* pixmapglob =  new TH2I("pixmapglob","pixmapglob", 8, 0, 8, 8, 0, 8);
   TH2I* pixmapclus =  new TH2I("pixmapclus","pixmapclus", 8, 0, 8, 8, 0, 8);

   Long64_t nbytes = 0, nb = 0;
   unsigned int totevts=0;
   
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      
      totevts++;
      
      const int ped = 50; // Pedestal is at 50
      const int thr = 75; // Threshold to consider signals
      
      unsigned int sum=0, sumclust=0;
      unsigned int sumsatur=0;  // count number of saturated channels in the event
      unsigned int max_v=0, max_i=99, max_chan=99;
      double posglobx=0., posgloby=0., posclusx=0., posclusy=0.;
      double chglob=0., chclus=0.;
      for (unsigned int i=0; i<channelDataLG->size(); i++) {
        if(channelDataLG->at(i) > 3900) sumsatur++;
	if(channelDataLG->at(i) > max_v) {
          max_v = channelDataLG->at(i);
          max_i = i;
          max_chan = channelID->at(i);
        }
      }
      if (max_i>63) std::cout << "ERROR in max index" << std::endl;
      if (max_chan>63) std::cout << "ERROR in max channel index" << std::endl;
      
      
      // Uncomment the line below to remove events with more than 2 saturated channels
      // if (sumsatur>2) continue;  // Remove events with more than 2 saturated channels
      
      for (unsigned int i=0; i<channelDataLG->size(); i++) {
        if (channelDataLG->at(i) < thr) continue;
	
	sum += channelDataLG->at(i)-ped;       
        ampliDistLG[channelID->at(i)]->Fill(channelDataLG->at(i)-ped);
	
//	pixmapglob->Fill(chID2mapX[channelID->at(i)], chID2mapY[channelID->at(i)], (channelDataLG->at(i)-ped)/4000.);
	pixmapglob->Fill(chID2mapX[channelID->at(i)], chID2mapY[channelID->at(i)], channelDataLG->at(i)/4000.);  // Subtracting the pedestal sets a negative offset in many bins

	chglob += channelDataLG->at(i)-ped;
	posglobx += (channelDataLG->at(i)-ped) * chID2mapX[channelID->at(i)];
	posgloby += (channelDataLG->at(i)-ped) * chID2mapY[channelID->at(i)];

        // Analyze main cluster only if max in 4 central pixels
//        if ( (chID2mapX[max_chan]==3||chID2mapX[max_chan]==4) && (chID2mapY[max_chan]==3||chID2mapY[max_chan]==4) ) {  
        if ( abs(chID2mapX[channelID->at(i)]-chID2mapX[max_chan])<2 && abs(chID2mapY[channelID->at(i)]-chID2mapY[max_chan])<2 ) {
	  sumclust += channelDataLG->at(i)-ped;
	  
//          pixmapclus->Fill(chID2mapX[channelID->at(i)], chID2mapY[channelID->at(i)], (channelDataLG->at(i)-ped)/4000.);
          pixmapclus->Fill(chID2mapX[channelID->at(i)], chID2mapY[channelID->at(i)], channelDataLG->at(i)/4000.);  // Subtracting the pedestal sets a negative offset in many bins

	  chclus += channelDataLG->at(i)-ped;
	  posclusx += (channelDataLG->at(i)-ped) * chID2mapX[channelID->at(i)];
	  posclusy += (channelDataLG->at(i)-ped) * chID2mapY[channelID->at(i)];
	}
//        }
            
      }
 
//   if (max_v<3900 ) {    // Cut saturated events from plots
     
     // Conversion from pixel number to coordinate (mm)
     // Pixel number ranges from 0 to 7
     // The center of the detector, in term of pixel number, is at the position (3.5,3.5)
     
     if ((sumclust)>0) MainClustCharge->Fill(sumclust);
     MainPixCharge->Fill(channelDataLG->at(max_i)-ped);
     TotalCharge->Fill(sum); 
     xglob->Fill(((posglobx/chglob)-3.5)*pix_size);
     yglob->Fill(((posgloby/chglob)-3.5)*pix_size);
     xclus->Fill(((posclusx/chclus)-3.5)*pix_size);
     yclus->Fill(((posclusy/chclus)-3.5)*pix_size);
     xyglob->Fill(((posglobx/chglob)-3.5)*pix_size,((posgloby/chglob)-3.5)*pix_size);
     xyclus->Fill(((posclusx/chclus)-3.5)*pix_size,((posclusy/chclus)-3.5)*pix_size);
     xyglobloose->Fill(((posglobx/chglob)-3.5)*pix_size,((posgloby/chglob)-3.5)*pix_size);
     xyclusloose->Fill(((posclusx/chclus)-3.5)*pix_size,((posclusy/chclus)-3.5)*pix_size);
     
//   }
   
   } // event loop
   
   std::cout << "Processed data file with " << totevts << " events" << std::endl;
   
   outfile->Write();
   outfile->Close();

   
}
