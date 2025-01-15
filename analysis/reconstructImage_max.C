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

void reconstructImage_max(TString file_name) {
    
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

    // mappa per fare il pixel con maggior conteggio
    std::map<Int_t, EventData> eventMax;

    // Loop sugli eventi
    Long64_t nEntries = tree_channels->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree_channels->GetEntry(i);
        
        // eventMax
        if (dNumber > eventMax[eventID].nN)
        {
            eventMax[eventID].nX = dIndexX;
            eventMax[eventID].nY = dIndexY;
            //cout << dNumber << "\t" << dIndexX << "\t" << dIndexY << endl;
        }
        eventMax[eventID].nN += dNumber;
    }
    
    Int_t nBinX = 50;
    Int_t nBinY = nBinX;
    Float_t startX = -1.5;
    Float_t startY = startX;
    Float_t endX = 8.5;
    Float_t endY = endX;
    TH2F *hMax = new TH2F("hMax","SiPM hMax",nBinX,startX,endX,nBinY,startY,endY);
    hMax->GetXaxis()->SetTitle("X bin number (= 3 mm)");
    hMax->GetYaxis()->SetTitle("Y bin number (= 3 mm)");
    hMax->GetZaxis()->SetTitle("Counts");
    
    for (const auto& [event, anEvent] : eventMax) {
//    cout << anEvent.nX << " \t" << anEvent.nY << "\t" << anEvent.nN << endl;
    	hMax->Fill(anEvent.nX,anEvent.nY,anEvent.nN);
    }
	cout << "hMax integral \t" << hMax->Integral() << endl;
    
    TCanvas *c2 = new TCanvas("c2","c2");
    c2->cd();
	hMax->SetStats(kTRUE);
    hMax->Draw("colz");
	c2->SaveAs("hMax.png");
    
    // Chiudi il file
//     file->Close();
    return;
}

