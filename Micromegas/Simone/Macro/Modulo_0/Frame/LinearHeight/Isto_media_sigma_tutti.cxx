#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Modulo_0/LinearH/Prime_misure_modulo_0/Frame_modulo_0/"
#define outpath "/home/atlas/Micromegas/Simone/Output/Modulo_0/LinearH/Isto_media_sigma/"

void Isto_media_sigma_tutti (TString scan){

	// style option
	
	gStyle->SetPalette(1);
	gStyle->SetOptStat(111111);
	gStyle->SetPaintTextFormat(".3f");
	
	// file containing the data
	ostringstream file1;
	file1 << inpath  << scan ;  
	
	// create all the histograms

	TH1F *frame1 = new TH1F("Punto1","punto1", 20, 9.8, 10.1 );

	cout << "histos created " << endl;

	//variables for reading the file
	
	Float_t x,y,z;
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
		
	
		in >> x >> y >> z;


		cout <<"x" << " " <<  x<< endl;
                cout <<"y" << " " <<  y<< endl;  
                cout <<"z" << " " <<  z<< endl;   

                frame1->Fill(x);
                frame1->Fill(y);
		frame1->Fill(z);

		x = 0;
                y = 0;
                z = 0;

		
		
		in >> date >> time; 
		cout << "date " << date <<" time " << time << endl; 

		
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
	Smedia1 << outpath <<"myout_" << scan << "_Isto_media_sigma.png";


	// create a canvas for each plot and drawing with different options
	TCanvas *c0 = new TCanvas();
	frame1->SetLineColor(kRed);
	frame1->Draw();
	frame1->Write();
	c0->Print(Smedia1.str().c_str());

	
 
	//myroot.Close();


  return;
}
