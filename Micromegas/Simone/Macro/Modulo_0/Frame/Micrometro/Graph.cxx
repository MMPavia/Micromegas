#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Macro/Modulo_0/Frame/Micrometro/"
#define outpath "/home/atlas/Micromegas/Simone/Macro/Modulo_0/Frame/Micrometro/"

// Macro per la costruzione di grafici dei lati della frame


void Graph (TString scan){

	// style option
	
//	gStyle->SetPalette(1);
//	gStyle->SetOptStat(111111);
	gStyle->SetPaintTextFormat(".3f");
	
	// file containing the data
	ostringstream file1;
	file1 << inpath  << scan ;  
	

	//variables for reading the file
	int r = 12;
 	Double_t x[r];
   	Double_t  s[r],t[r],u[r];
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
        int j=0;

	// looping on the file lines
	while (1) {
		
		in >> nmis;
		
		if ( !in.good() || in.eof()) break;  // another sanity check

	

        
       		in >> s[j];
       		x[j]=nmis*10;

       		cout <<"x= " << x[j] << endl;
    		cout <<"s= " << s[j] << endl;

         
		j++;
		
	} // end while





	cout << "file completed "<< endl;


	// close the input file
	in.close();

        cout << "file closed "<< endl;

        //Creazione Graph
        TGraph *gr1 = new TGraph (j,x,s);

        cout << "graph created"<< endl;

	// create a root file for the histograms; 
	ostringstream rootfile;
	rootfile  << outpath <<"myout_"<<scan <<".root";
	TFile myroot(rootfile.str().c_str(), "RECREATE");
	cout << "Root output file: " << endl << rootfile.str() << endl;


  	 TMultiGraph *mg = new TMultiGraph();



	 mg->Add(gr1); gr1->SetTitle("Lato 1")  ;		 gr1->SetLineWidth(3); gr1->SetLineColor(kRed);



	TCanvas *c1 = new TCanvas("c1","Graph Draw Options",200,10,600,400);
 	mg->Draw("AC*");


	 leg = new TLegend(0.1,0.7,0.48,0.9);
  	 leg->SetHeader("Larghezza frame");
  	 leg->AddEntry(gr1,"Lato 1");
//         leg->AddEntry(gr2,"Lato 2");
  	 leg->Draw();

	
	
	//3yroot.Close();


  return;
}
