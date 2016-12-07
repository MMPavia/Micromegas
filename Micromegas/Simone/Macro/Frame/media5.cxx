#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Frame_3_punti/"
#define outpath "/home/atlas/Micromegas/Simone/Output/"

void media5 (TString scan){

	// style option
	
	gStyle->SetPalette(1);
	gStyle->SetOptStat(111111);
	gStyle->SetPaintTextFormat(".3f");
	
	// file containing the data
	ostringstream file1;
	file1 << inpath  << scan ;  
	
	// create all the histograms

	TH1F *frame1 = new TH1F("Punto1","punto1", 20, 9.90, 10.10 );
        TH1F *frame2 = new TH1F("Punto2","punto2", 20, 9.90, 10.10 );
	TH1F *frame3 = new TH1F("Punto3","punto3", 20, 9.90, 10.10 );

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
                frame2->Fill(y);
		frame3->Fill(z);

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


	// create a canvas for each plot and drawing with different options
	TCanvas *c0 = new TCanvas();
	frame1->SetLineColor(kRed);
	frame1->Draw();
	frame1->Write();

	TCanvas *c1 = new TCanvas();
	frame2->SetLineColor(kBlue);
        frame2->Draw("SAME");
        frame2->Write();

	TCanvas *c2 = new TCanvas();
	frame3->SetLineColor(kGreen);
	frame3->Draw("SAME");
        frame3->Write();








	
 
	//myroot.Close();


  return;
}
