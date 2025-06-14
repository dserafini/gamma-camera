// Info:
// root 'analysis/ntupSimToFers.cc("tutorial.root","fersTreeNtuple.root")'
// script to move from geant4 output to exp output
// so that the same analysis macro can be used
// written by Matteo Negrini

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <iostream>
#include <map>
#include <vector>

struct EventData {
    std::vector<UShort_t> chan;
    std::vector<UShort_t> val;
};

UShort_t XYtoFersChan(Int_t iX, Int_t iY) {
    // valori di X  dei canali 0-63   
    size_t const chID2mapX[64] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 
                                  0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7};
  
    //Valori di Y tra i canali 0-63 
    size_t const chID2mapY[64] = {6, 5, 7, 4, 7, 4, 6, 5, 6, 5, 7, 4, 7, 4, 6, 5, 6, 5, 7, 4, 7, 4, 6, 5, 6, 5, 7, 4, 7, 4, 6, 5, 
                                  2, 1, 3, 0, 3, 0, 2, 1, 2, 1, 3, 0, 3, 0, 2, 1, 2, 1, 3, 0, 3, 0, 2, 1, 2, 1, 3, 0, 3, 0, 2, 1};
				  
    for (UShort_t ch=0; ch<64; ch++) {
      if (chID2mapX[ch]==iX && chID2mapY[ch]==iY) return ch;
    }
    
    return 99;

}

void ntupSimToFers(TString file_in_name, TString file_out_name) {
  
    // Apri il file ROOT di input
    TFile* file = TFile::Open(file_in_name, "READ");
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


    // mappa per raccogliere i dati
    std::map<Int_t, EventData> eventData;

    // Loop sugli eventi
    Long64_t nEntries = tree_channels->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree_channels->GetEntry(i);
        
	eventData[eventID].chan.push_back(XYtoFersChan(dIndexX, dIndexY));
	eventData[eventID].val.push_back(dNumber);
	
    }
    
    // Apri il file ROOT di output
    TFile* fileout = TFile::Open(file_out_name, "recreate");
    TTree* fers_tree = new TTree("fersTree","fersTree");
    
    UInt_t trigID;
    float trigTime;
    vector<UShort_t>* channelID;
    vector<UShort_t>* channelDataHG;
    vector<UShort_t>* channelDataLG;
  
    TBranch* trigID_b;
    TBranch* trigTime_b;
    TBranch* channelID_b;
    TBranch* channelDataLG_b;
    TBranch* channelDataHG_b;
    
    trigID_b = fers_tree->Branch("trigID", &trigID);
    trigTime_b = fers_tree->Branch("trigTime", &trigTime);
    channelID_b = fers_tree->Branch("channelID", &channelID);
    channelDataLG_b = fers_tree->Branch("channelDataLG", &channelDataLG);
    channelDataHG_b = fers_tree->Branch("channelDataHG", &channelDataHG);
    
    int evtcount=0;
    
    for (auto& [event, anEvent] : eventData) {
    	trigID = event;
	trigTime = 0;
	channelID = &(anEvent.chan);
	channelDataLG = &(anEvent.val);
	channelDataHG = &(anEvent.val);
// 	std::cout << " -- event: " << event;
// 	for (auto& theval : anEvent.val) std::cout << " theval: " << theval;
// 	std::cout << std::endl;
        fers_tree->Fill();
	evtcount++;
    }
    
    fers_tree->Write();
    std::cout << "Writing output file with " << evtcount << " events." << std::endl;

    
    // Chiudi i files
    file->Close();
    fileout->Close();
    return;
}

