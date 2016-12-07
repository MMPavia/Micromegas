#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Frame_incollatrice/"
#define outpath "/home/atlas/Micromegas/Simone/Output/"

void Isto_Largh_frame_incollatrice (TString scan){

	// style option
	
	gStyle->SetPalette(1);
	gStyle->SetOptStat(111111);
	gStyle->SetPaintTextFormat(".3f");
	
	// file containing the data
	ostringstream file1;
	file1 << inpath  << scan ;  
	
	// create all the histograms

	TH1F *frame = new TH1F("frame","frame", 20, 25.90, 26.00 );

	cout << "histos created " << endl;

	//variables for reading the file
	
	Float_t x[12],u;
	Int_t nmis,j=0;
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
		
		in >> nmis >> u >> u >> u;
		
		if ( !in.good() || in.eof()) break;  // another sanity check

		cout <<"nmis " << nmis << endl;
		
		
			in >> x[j];
			x[j]= (x[j]+ 26.005);
			cout <<"x[" <<j << "] " <<  x[j]<< endl; 
			frame->Fill(x[j]);
		
		in >> date >> time; 
		cout << "date " << date <<" time " << time << endl; 
		j++;
		
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
