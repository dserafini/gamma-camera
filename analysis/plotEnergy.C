#include <TFile.h>
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

void plotEnergy(TString file_name) {
    
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

    // mappa per fare il totale di conteggi per evento
    std::map<Int_t, EventMean> eventTotal;

    // histo 2d per canali e 1d per cumulativo
    Int_t nBinX = 500;
    Float_t startX = -.5;
    Float_t endX = 5000 - startX;
    TH1F *hEnergy = new TH1F("hEnergy","SiPM counts per event",nBinX,startX,endX);
    hEnergy->GetXaxis()->SetTitle("dNumber");
    hEnergy->GetYaxis()->SetTitle("Counts");
    TH2F *hChannelEnergy = new TH2F("hChannelEnergy","SiPM counts per event per channel",nBinX,startX,endX,64,-.5,63.5);
    hChannelEnergy->GetXaxis()->SetTitle("dNumber");
    hChannelEnergy->GetYaxis()->SetTitle("Channel");
    hChannelEnergy->GetZaxis()->SetTitle("Counts");

    // Loop sugli eventi
    Long64_t nEntries = tree_channels->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree_channels->GetEntry(i);
        
        hChannelEnergy->Fill(dNumber, dIndexY*8 + dIndexX);
        eventTotal[eventID].nN += dNumber;
	    eventTotal[eventID].dX += dIndexX * dNumber;
	    eventTotal[eventID].dY += dIndexY * dNumber;
    }
    
    // normalizza la mappa eventTotal
    for (auto& [event, anEvent] : eventTotal) {
    	anEvent.dX /= anEvent.nN;
    	anEvent.dY /= anEvent.nN;
    	eventTotal[event] = anEvent;
    	hEnergy->Fill(anEvent.nN);
    }
	cout << "hEnergy integral \t" << hEnergy->Integral() << endl;
    
    TCanvas *c1 = new TCanvas("c1","c1");
    c1->cd();
	hChannelEnergy->SetStats(kFALSE);
    hChannelEnergy->Draw("colz");
	c1->SaveAs("hChannelEnergy.png");

    TCanvas *c2 = new TCanvas("c2","c2");
    c2->cd();
	hEnergy->SetStats(kTRUE);
    hEnergy->GetXaxis()->SetRangeUser(0,2000);
    hEnergy->Draw("");
	c2->SaveAs("hEnergy.png");
    
    // Chiudi il file
//     file->Close();
    return;
}

