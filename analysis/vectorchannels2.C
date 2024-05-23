THStack* vectorchannels2(TString fileName = "Cs137average100kevents.root", Int_t testStackElement = 0, Bool_t save = false)
{
	TH1::AddDirectory(kFALSE); // do not save histograms in directory
	const Int_t nBinsX = 8;
	const Int_t maxCounts = 7000;
	THStack* hPixels = new THStack("hPixels","hPixels");
	THStack *hEvents = new THStack("hEvents","hEvents");
	THStack *hSelectedEvents = new THStack("hSelectedEvents","hSelectedEvents");
	TH2F* hAllEvents;
	TH1F* hAllPixels;
	TH1F* hAllPixelsForSelectedEvents;
	TH1F* hFourCentralPixels;
	TH1F* h16CentralPixels;
	TH1F* hOneCentralPixels;
	TH1F* hOneLateralPixels;
	TH2F* hEdepVsCounts = new TH2F("hEdepVsCounts","hEdepVsCounts",70,0,maxCounts,70,0,700);
	hEdepVsCounts->GetXaxis()->SetTitle("dNumber tot");
	hEdepVsCounts->GetYaxis()->SetTitle("edep (keV)");
	hAllPixels = new TH1F("hAllPixels","hAllPixels",maxCounts/10,-.5,maxCounts-.5);
	hAllPixels->SetTitle("nEvents / counts for all pixels");
	hAllPixels->SetLineColor(kBlack);
	hAllPixels->GetXaxis()->SetTitle("n Counts");
	hAllPixels->GetYaxis()->SetTitle("n Events");
	hFourCentralPixels = (TH1F*)hAllPixels->Clone("hFourCentralPixels");
	hFourCentralPixels->SetTitle("nEvents / counts for four central pixels");
	hFourCentralPixels->SetLineColor(kGreen);
	h16CentralPixels = (TH1F*)hAllPixels->Clone("h16CentralPixels");
	h16CentralPixels->SetTitle("nEvents / counts for 16 central pixels");
	h16CentralPixels->SetLineColor(kCyan);
	hOneCentralPixels = (TH1F*)hAllPixels->Clone("hOneCentralPixels");
	hOneCentralPixels->SetTitle("nEvents / counts for one central pixels");
	hOneCentralPixels->SetLineColor(kTeal);
	hOneLateralPixels = (TH1F*)hAllPixels->Clone("hOneLateralPixels");
	hOneLateralPixels->SetTitle("nEvents / counts for one lateral pixels");
	hOneLateralPixels->SetLineColor(kMagenta);
	hAllPixelsForSelectedEvents = (TH1F*)hAllPixels->Clone("hAllPixelsForSelectedEvents");
	hAllPixelsForSelectedEvents->SetTitle("nEvents / counts for all pixels for selected events");
	hAllPixelsForSelectedEvents->SetLineColor(kBlue);
	hAllEvents = new TH2F("hAllEvents", "hAllEvents", nBinsX, -.5, nBinsX-.5, nBinsX, -.5, nBinsX-.5);
	hAllEvents->GetXaxis()->SetTitle("x bins");
	hAllEvents->GetYaxis()->SetTitle("y bins");
	hAllEvents->GetZaxis()->SetTitle("counts");
	TList *l2;
	Float_t gainMatrix[nBinsX][nBinsX];
	
	hAllPixels->SetDirectory(gDirectory);
	hEdepVsCounts->SetDirectory(gDirectory);
	
	// set gain matrix
	cout << "Gain matrix" << endl;
	TRandom *r1 = new TRandom();
	const Float_t halfRandomWidth = 0.0;
	for (int iy=0; iy< nBinsX; iy++)
	{
		for (int ix=0; ix< nBinsX; ix++)
		{
			gainMatrix[ix][iy] = 1.; // by default all pixels have the same gain
			gainMatrix[ix][iy] += r1->Uniform(-halfRandomWidth,halfRandomWidth);
			cout << TString::Format("%.2f\t",gainMatrix[ix][iy]);
		}
		cout << endl;
	}
	
	TFile* files = TFile::Open(fileName);
	if (!files) cout << fileName << " not found!" << endl;
	else
	{
		TTree* tgen = (TTree*)files->Get("Generation");
		if (!tgen) cout << "Generation tree not found in " << fileName << "!" << endl;
		else
		{
			const int nGenerationEvents = tgen->Draw("eventID", "", "goff");
			Double_t *eventsGenerationArray = tgen->GetV1();
			cout << "Generation tree has " << nGenerationEvents << " entries" << endl;
			
			TTree* tscin = (TTree*)files->Get("Scintillator");
			if (!tscin) cout << "Scintillator tree not found in " << fileName << "!" << endl;
			else
			{
				const int nScintillatorEdep = tscin->Draw("fEdep", "", "goff");
				Double_t *edepScintillatorArray = tscin->GetV1();
				cout << "Scintillator tree has " << nScintillatorEdep << " entries" << endl;
				
				if (nGenerationEvents != nScintillatorEdep)
					cout << nGenerationEvents << " != " << nScintillatorEdep << "!" << endl;
				else
				{
					TH2F *hOneEvent;
					TTree* tcha = (TTree*)files->Get("Channels");
					if (!tcha) cout << "Channels tree not found in " << fileName << "!" << endl;
					else
					{
						// save vectors from trees
						cout << "reading vectors from trees" << endl;
						const int nChannelsEvents = tcha->GetEntries();
						cout << "Channels tree has " << nChannelsEvents << " entries" << endl;
						vector <Double_t> eventsChannelsVector(nChannelsEvents);
						vector <Double_t> dNumberChannelsVector(nChannelsEvents);
						vector <Double_t> dIndexXChannelsVector(nChannelsEvents);
						vector <Double_t> dIndexYChannelsVector(nChannelsEvents);
						tcha->SetEstimate(-1); // essenziale o Draw si ferma a 1M
						tcha->Draw("eventID", "", "goff");
						for (int i=0; i<nChannelsEvents; i++)
							eventsChannelsVector.at(i) = tcha->GetV1()[i];
						tcha->Draw("dIndexX:dIndexY:dNumber", "", "goff");
						for (int i=0; i<nChannelsEvents; i++)
						{
							dIndexXChannelsVector[i] = tcha->GetV1()[i];
							dIndexYChannelsVector[i] = tcha->GetV2()[i];
							dNumberChannelsVector.at(i) = tcha->GetV3()[i];
						}
						
						// Double_t maxCountsForOnePixel = *max_element(dNumberChannelsVector.begin(), dNumberChannelsVector.end());
						
						// fill the TH2 from the tree vectors
						cout << "filling the TH2 from the tree vectors" << endl;
						gStyle->SetOptStat(0);
						int j=0;
						int countsPerEvent = 0;
						for (int i=0; i<nGenerationEvents; i++)
						{
							TString histoname = TString::Format("event%d",(Int_t)eventsGenerationArray[i]);
							hOneEvent = (TH2F*)hAllEvents->Clone(histoname);
							hOneEvent->Reset();
							hOneEvent->SetTitle(fileName + " " + TString(std::to_string((Int_t)eventsGenerationArray[i])) + " edep " + TString(std::to_string((Int_t)edepScintillatorArray[i])) + " keV");
							Int_t countsForOneEvent = 0;
							while (j<nChannelsEvents && (eventsChannelsVector.at(j) == eventsGenerationArray[i]))
							{
								countsForOneEvent += dNumberChannelsVector.at(j);
								Int_t theIndexX = dIndexXChannelsVector.at(j);
								Int_t theIndexY = dIndexYChannelsVector.at(j);
								hOneEvent->Fill(theIndexX,theIndexY,dNumberChannelsVector.at(j)*gainMatrix[theIndexX][theIndexY]);
								// cout << hOneEvent->GetEntries() << "\t " << hOneEvent->Integral() << endl;
								j++;
							}
							hEvents->Add((TH2F*)hOneEvent->Clone()); // questo va bene anche prima
							hAllEvents->Add(hOneEvent); // questo va bene solo dopo penso
							hEdepVsCounts->Fill(countsForOneEvent,edepScintillatorArray[i]);
							hAllPixels->Fill(hOneEvent->Integral());
							hFourCentralPixels->Fill(hOneEvent->Integral(4,5,4,5));
							h16CentralPixels->Fill(hOneEvent->Integral(3,6,3,6));
							hOneCentralPixels->Fill(hOneEvent->Integral(4,4,4,4));
							hOneLateralPixels->Fill(hOneEvent->Integral(3,3,3,3));
							if (edepScintillatorArray[i] > 600)
							{
								hSelectedEvents->Add(hOneEvent);
								hAllPixelsForSelectedEvents->Fill(hOneEvent->Integral());
							}
						}
						
						cout << "plotting the results" << endl;
						TCanvas *c1 = new TCanvas("c1","c1",800,700);
						gPad->SetRightMargin(0.2);
						c1->SetLogy(kFALSE);
						c1->cd();
						
						if (l2)
						{
							TH1F* tf;
							TLegend *leg1 = new TLegend(.5,.5,.8,.8);
							for (TObject* to : *l2)
							{
								tf = (TH1F*)to;
								tf->Draw("SAME");
								leg1->AddEntry(tf,tf->GetName());
							}
							leg1->Draw();
						}
						//hMatrix->Draw("colz");
						
						TLegend *leg2 = new TLegend(.75,.2,.95,.95);
						hAllPixels->SetTitle("nEvents / counts");
						hAllPixels->Draw("");
						// hAllPixelsForSelectedEvents->Draw("SAME");
						hFourCentralPixels->Draw("SAME");
						h16CentralPixels->Draw("SAME");
						hOneCentralPixels->Draw("SAME");
						hOneLateralPixels->Draw("SAME");
						leg2->AddEntry(hAllPixels,"All");
						leg2->AddEntry(h16CentralPixels,"Sixteen central");
						leg2->AddEntry(hFourCentralPixels,"Four central");
						leg2->AddEntry(hOneCentralPixels,"One central");
						leg2->AddEntry(hOneLateralPixels,"One lateral");
						leg2->Draw();
						hEdepVsCounts->Draw("colz");
						if (save) c1->SaveAs("TH1" + fileName + ".png");
					}
				}
			}
		}
		files->Close();
	}
	return hSelectedEvents;
}
