#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/M0Data/mapping/"
#define outpath "/home/atlas/Micromegas/Simone/Output/Modulo_0/CMM/"

void Isto_Frame_CMM (TString scan){

	// style option
	
//	gStyle->SetPalette(1);
//	gStyle->SetOptStat(111111);
	gStyle->SetPaintTextFormat(".3f");
	
	// file containing the data
	ostringstream file1;
	file1 << inpath  << "mapping_2016012211002_ref.txt" ;  
	

	//variables for reading the file
	int r = 36;
 	Double_t x[r];
   	Double_t  s[r],t[r],u;
   	Int_t nmis;
   	string date, time;

	
	// reading the file
	ifstream in(file1.str().c_str());
	
 	 if (!in || in.bad() )
	 {
		cout << "file non letto" << endl;
		return; // sanity check
	}
	
//	cout << "starting reading file " << endl;
        int j=0;

	// looping on the file lines
	while (1) {
		
		in >> nmis >> u  >> s[j]  >> u;
		
		if ( !in.good() || in.eof()) break;  // another sanity check


       		in >> x[j];
		x[j]= x[j]-s[j];
		s[j]=0;
//       		cout <<"x= " << x[j] << endl;
                in >> date >> time; 
		j++;
		
	} // end while



	cout << "file reference completed "<< endl;


	// close the input file
       	in.close();
//  	 cout << "file chiuso "<< endl;
	

//	cout << scan << endl;
	ostringstream gianni;
        
	gianni << inpath  << scan;
//	cout << "aperto file di lettura" <<endl;

        TH1F *frame1 = new TH1F("Punto1","punto1", 20, 9.8, 10.06 );
//	cout << "Creato istogramma" << endl;

	
	//sanity check

        ifstream in1(gianni.str().c_str());

         if (!in1 || in1.bad() )
         {
                cout << "file non letto" << endl;
                return; // sanity check
        }


//        cout << "starting reading file " << endl;
        j=0;


        while (1) {

                in1 >> nmis >> u  >> s[j]  >> u ;

                if ( !in1.good() || in1.eof()) break;  // another sanity check




                in1 >> t[j];
                t[j]= (t[j]-s[j]-x[j]);

//		cout <<"reference[j]= " << x[j] << endl;
//                cout <<"s= " << t[j] << endl;
//                cout <<"nmis " << nmis << endl;

                frame1->Fill(t[j]);

                in1 >> date >> time;
//                cout << "date " << date <<" time " << time << endl;
                j++;

        }

        in1.close();




/*
        //Creazione Graph
        TGraph *gr1 = new TGraph (12,x,s);
        TGraph *gr2 = new TGraph (12,x,t);


	// create a root file for the histograms; 
	ostringstream rootfile;
	rootfile  << outpath <<"myout_"<<scan <<".root";
	TFile myroot(rootfile.str().c_str(), "RECREATE");
	cout << "Root output file: " << endl << rootfile.str() << endl;


  	 TMultiGraph *mg = new TMultiGraph();



	 mg->Add(gr1); gr1->SetTitle("Lato 1")  ;		 gr1->SetLineWidth(3); gr1->SetLineColor(kRed);
  	 mg->Add(gr2); gr2->SetTitle("Lato 2");		 gr2->SetLineWidth(3); gr2->SetLineColor(kBlue);



	TCanvas *c1 = new TCanvas("c1","Graph Draw Options",200,10,600,400);
 	mg->Draw("AC*");


	 leg = new TLegend(0.1,0.7,0.48,0.9);
  	 leg->SetHeader("Larghezza frame");
  	 leg->AddEntry(gr1,"Lato 1");
         leg->AddEntry(gr2,"Lato 2");
  	 leg->Draw();

	
	
	//3yroot.Close();
*/
        ostringstream Smedia1;
        Smedia1 << outpath <<"myout_" << scan << "_Isto_Frame_Cmm.png";


//        create a canvas for each plot and drawing with different options
        TCanvas *c0 = new TCanvas();
        frame1->SetLineColor(kRed);
        frame1->Draw();
        frame1->Write();
        c0->Print(Smedia1.str().c_str());



  return;
}
