#include "TFile.h"
#include <TTree.h>
#include <iostream>
#include <map>

struct EventData {
    int nN;
    int nX;
    int nY;
    
    // costruttore
    // EventData(int dNumber, int dIndexX, int dIndexY) : nN(dNumber), nX(dIndexX), nY(dIndexY) {}
	// niente costruttore, altrimenti non riesco a mapparlo
};

struct EventMean {
    int nN;
    double dX;
    double dY;
};

void reconstructImage_sum(TString file_name) {
    
    // Apri il file ROOT
    TFile* file = TFile::Open(file_name, "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Errore nell'aprire il file!" << std::endl;
        return;
    }

    // Prendi l'albero dal file
    TTree* tree_channels = (TTree*)file->Get("Channels");
    if (!tree_channels) {
        std::cerr << "Errore nel trovare l'albero!" << std::endl;
        file->Close();
        return;
    }

    // Variabili per leggere i rami
    Int_t eventID;
    Int_t dNumber;
    Int_t dIndexX;
    Int_t dIndexY;

    // Collega i rami alle variabili
    tree_channels->SetBranchAddress("eventID", &eventID);
    tree_channels->SetBranchAddress("dNumber", &dNumber);
    tree_channels->SetBranchAddress("dIndexX", &dIndexX);
    tree_channels->SetBranchAddress("dIndexY", &dIndexY);
    
    Int_t nBinX = 50;
    Int_t nBinY = nBinX;
    Float_t startX = -1.5;
    Float_t startY = startX;
    Float_t endX = 8.5;
    Float_t endY = endX;
    TH2F *hSum = new TH2F("hSum","hSum",nBinX,startX,endX,nBinY,startY,endY);

    // Loop sugli eventi
    Long64_t nEntries = tree_channels->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree_channels->GetEntry(i);
    	hSum->Fill(dIndexX,dIndexY,dNumber);
    }
	cout << "hSum integral \t" << hSum->Integral() << endl;
    
    TCanvas *c2 = new TCanvas("c2","c2");
    c2->cd();
	hSum->SetStats(kFALSE);
    hSum->Draw("colz");
	c2->SaveAs("hSum.png");
    
    // Chiudi il file
//     file->Close();
    return;
}

