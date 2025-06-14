#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "fersRead.h"

using namespace std;

ClassImp(fersRead)

fersRead::fersRead(string ifilename, string ofilename, string gain) 
: ifilename_        (ifilename), 
  ofilename_        (ofilename),
  gain_             (gain),
  otreename_        ("fersTree"),
  otree_            (0),
  debugger_         (0),
  eventCounter_     (0), 
  nLineRead_       (0) {
  
  cout << " Pronto a leggere il file " << ifilename << endl;
}

void fersRead::read() {
  
  if (ifilename_.empty()) { cout << " Please use SetInputFile(\"INPUT file\")" << endl; return; }
  
  if (ofilename_.empty()) {
    
    string::size_type start =  ifilename_.find("Run");
    string::size_type stop =  ifilename_.find("_");
    cout<<" Start position "<< start <<"; stop position "<< stop<<endl;
 
   
    string runNumber  (ifilename_.substr(start+3, stop -(start+3))); // 
 
    cout << " runNumber     : " << runNumber      << endl;	
    
    ofilename_ =  ifilename_.substr(0, start)+ "run_" + runNumber + ".root";     
  }

  cout << " ifilename: " << ifilename_ << endl;
  cout << " ofilename: " << ofilename_ << endl;
  cout << " selected gain: "<< gain_ << endl; 
  
  ifstream ifile(ifilename_.c_str()); if (!ifile)           { cout << " Cannot open: " << ifilename_ << endl; return; }  
  TFile    ofile(ofilename_.c_str(), "UPDATE");    if (ofile.IsZombie()) { cout << " Cannot open: " << ofilename_ << endl; return; }
  
  otree_ = dynamic_cast<TTree*>(ofile.Get(otreename_.c_str()));            // Try to get the tree from the file
  if (!otree_) otree_ = new TTree(otreename_.c_str(), otreename_.c_str()); // If it doesn't exist, create it

  t.init(otree_);
  t.reset();

  eventCounter_ = 0;
  nLineRead_   = 0;
  string input_line;
  
  while (getline(ifile, input_line)) {
  
    ++nLineRead_;
    if(eventCounter_ < 3) cout<<input_line<<endl;
     
    if (input_line.find("Tstamp_us")!=string::npos) continue;
    if (input_line.find("//")!=string::npos) continue;

    istringstream instring;
    instring.str(input_line);
    string timecheck;
    int channel=-1, datumLG=-1, datumHG=-1;
    UInt_t trig_id=0, board;
    string S1="";  //placeholder for "-" in input string
    
    instring>>timecheck; //first string in input_line 
    if (timecheck.find(".") == string::npos){  
        if(gain_ == "LG"){
           instring>>channel>>datumLG;
	   t.channelDataLG->push_back(datumLG);
        }else if(gain_ == "HG"){
           instring>>channel>>S1>>datumHG;
	   t.channelDataHG->push_back(datumHG);
        }else if(gain_ == "BOTH"){
	  instring>>channel>>datumLG>>datumHG;
	// can the case with both gain exist? then need to add another vector to the ntupla
	//if(eventCounter_ < 5)cout<<" channel="<<channel<<" counts="<<datum<<endl;
	  t.channelDataLG->push_back(datumLG); 
	  t.channelDataHG->push_back(datumHG);
	}  
        t.channelID->push_back(channel);
    }else{
        // new event; save previous one unless it is the first
        if (eventCounter_ > 0){
	  // data can contain a lower number of channels than available in FERS
	  //            if ( t.channelID -> size() != 64){
	  //                 cout<< "something wrong "<<endl;
	  //		 return;
	  //           }
            otree_->Fill();
	    t.reset();
        }
    	if(gain_ == "LG"){
	  instring>>trig_id>>board>>channel>>datumLG;
          t.channelDataLG->push_back(datumLG);     
	}else if(gain_ == "HG"){
	  instring>>trig_id>>board>>channel>>S1>>datumHG;
          t.channelDataHG->push_back(datumHG);     	
	}else if(gain_ == "BOTH"){
	  instring>>trig_id>>board>>channel>>datumLG>>datumHG;
          t.channelDataLG->push_back(datumLG);     
	  t.channelDataHG->push_back(datumHG);     
        }
	++eventCounter_;
	t.trigTime = std::atof(timecheck.c_str());
	t.channelID->push_back(channel);
	t.trigID = trig_id;
    }
  }

  
  otree_->SetTitle("fersData");
  
  ifile.close();

  otree_->Write("", TObject::kOverwrite);
  delete otree_;
  otree_ = 0;
  
  ofile.Close();

  cout << " nLineRead: " << nLineRead_    << endl;
  cout << " Events: "     << eventCounter_  << endl; 
}

