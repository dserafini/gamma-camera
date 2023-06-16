void scintiBin(TString fileName, Double_t scinti_hole_thickness = 8., Double_t scinti_septa_thickness = 2., Double_t case_side = 100., Int_t scinti_holes_number = 10)
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
      TCanvas * c1 = new TCanvas("c1","c1",10,10,800,800);
      c1->cd(1);
      
      // calculate number of bins
      Double_t scinti_pixel_size = scinti_hole_thickness + scinti_septa_thickness;
      scinti_holes_number = (Int_t) case_side / scinti_pixel_size;
      if ((scinti_holes_number % 2) < 1)
        scinti_holes_number = scinti_holes_number - 1;
	    case_side = (Double_t) scinti_pixel_size * scinti_holes_number;
      Double_t half_case_side = case_side/2.;
      
	TH2F* h2 = new TH2F("h2","h2",scinti_holes_number,-half_case_side,half_case_side,scinti_holes_number,-half_case_side,half_case_side);
	h2->GetXaxis()->SetTitle("x [mm]");
	h2->GetYaxis()->SetTitle("y [mm]");
	h2->SetTitle(fileName + " gamma fY:fX");
	cout << "scinti_pixel_size: " << h2->ProfileX()->GetBinWidth(1) << " mm" << endl;
	t1->Draw("fY:fX>>h2","fEdep>0", "colz");
	c1->SaveAs("c1scintiBin"+fileName+"GammaPos.png");
	    
      	TCanvas * c2 = new TCanvas("c2","c2",10,10,800,800);
	c2->cd(1);
	TH2F* h2b = new TH2F("h2b","h2b",scinti_holes_number,-half_case_side,half_case_side,scinti_holes_number,-half_case_side,half_case_side);
	h2b->SetTitle(fileName + " optical pMeanY:pMeanX");
	t1->Draw("pMeanY:pMeanX>>h2b","fEdep>0", "colz");
	c2->SaveAs("c2scintiBin"+fileName+"OpticalPos.png");
    }
  }
}
