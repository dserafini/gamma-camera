void outh2(TString fileName, Int_t xBins = 500, TString drawOption = "colz", Bool_t logz = false)
{
  TFile* file = new TFile(fileName);
  if (!file) cout << "No file found!" << endl;
  else
  {
    if (fileName.EndsWith(".root"))
      fileName.Resize(fileName.Length()-5);
    
    TTree* t1 = (TTree*)file->Get("Scoring");
    if (!t1) cout << "No tree found!" << endl;
    else
    {
      TCanvas * c1 = new TCanvas("c1","c1",1000,200,800,800);
      c1->cd(1);
      TH2F* h2 = new TH2F("h2","h2",xBins,-50,50,xBins,-50,50);
      t1->Draw("fX:fY>>h2","fEdep>0", "colz");
      if (!h2) cout << "No histogram extracted!" << endl;
      else
      {
//        h2->GetXaxis()->SetRangeUser(-50,50);
//        h2->GetYaxis()->SetRangeUser(-50,50);
        h2->GetXaxis()->SetTitle("x [mm]");
        h2->GetYaxis()->SetTitle("y [mm]");
        h2->SetTitle(fileName);
        if (logz)
          c1->SetLogz();
        h2->Draw(drawOption);
        c1->SaveAs("c1"+fileName+".png");
      }
    }
  }
}
