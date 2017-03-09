#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/M05Data/frameData/"
#define outpath "/home/atlas/Micromegas/M05Data/root_plot/frame/"

void LHFrame (TString scan){

	// style option
	
	gStyle->SetPalette(1);
	gStyle->SetOptStat(111111);
	gStyle->SetPaintTextFormat(".0f");
	
	// file containing the data
	ostringstream file1;
	file1 << inpath  << scan ;  
	
	// create all the histograms

	TH1F *frame = new TH1F("frame","frame", 20, 9.90, 10.10 );

	cout << "histos created " << endl;

	//variables for reading the file
	
	Float_t x;
	Int_t nmis;
	string date, time;

	
	// reading the file
	ifstream in(file1.str().c_str());
	
 	 if (!in || in.bad() )
	 {
		cout << "file non letto" << endl;
		return; // sanity check
	}
	
	cout << "starting reading file " << endl;

	// looping on the file lines
	while (1) {
		
		in >> nmis;
		
		if ( !in.good() || in.eof()) break;  // another sanity check

		cout <<"nmis " << nmis << endl;
		
			in >> x ;
			cout <<"x = "  <<  x<< endl; 
			frame->Fill(x);
			x = 0;
		
		
		in >> date >> time; 
		cout << "date " << date <<" time " << time << endl; 

		
	} // end while
	
	cout << "file completed "<< endl;


	// close the input file
	in.close();

/*
	// raw values: average and differences wrt to the average
	Float_t value = z_corr->GetMean();
	Float_t sigma = z_corr->GetRMS();
*/
	
	
	// create a root file for the histograms; 
	ostringstream rootfile;
	rootfile  << outpath <<"myout_"<<scan <<".root";
	TFile myroot(rootfile.str().c_str(), "RECREATE");
	cout << "Root output file: " << endl << rootfile.str() << endl;


	// create a canvas for each plot and drawing with different options
	TCanvas *c0 = new TCanvas();
	frame->Draw();
	frame->Write();
	
 
	//myroot.Close();


  return;
}

