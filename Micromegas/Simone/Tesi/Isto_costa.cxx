#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/Users/simonesottocornola/Documents/Uni/MicroMegas/Misure_qaqc/Tesi/"
#define outpath "/Users/simonesottocornola/Documents/Uni/MicroMegas/Misure_qaqc/Tesi/output"

void Isto_costa (TString scan){

	// style option
	
	gStyle->SetPalette(1);
	gStyle->SetOptStat(111111);
	gStyle->SetPaintTextFormat(".3f");
	
	// file containing the data
	ostringstream file1;
	file1 << inpath  << scan ;  
	
	// create all the histograms

	TH1F *frame1 = new TH1F("Punto1","punto1", 20, 25.88, 26.01 );

	cout << "histos created " << endl;

	//variables for reading the file
	
	Float_t x;
	Int_t nmis;

	
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
		
	
		in >> x;


		cout <<"x" << " " <<  x<< endl;

                frame1->Fill(x);


		
	} // end while
	
	cout << "file completed "<< endl;


	// close the input file
	in.close();






	
	
	// create a root file for the histograms; 
	ostringstream rootfile;
	rootfile  << outpath <<"myout_"<<scan <<".root";
	TFile myroot(rootfile.str().c_str(), "RECREATE");
	cout << "Root output file: " << endl << rootfile.str() << endl;

	ostringstream Smedia1;
	Smedia1 << outpath <<"myout_" << scan << "_Isto_costa.png";


	// create a canvas for each plot and drawing with different options
	TCanvas *c0 = new TCanvas();
	frame1->SetLineColor(kRed);
	frame1->Draw();
	frame1->Write();
	c0->Print(Smedia1.str().c_str());

	
 
	//myroot.Close();


  return;
}
