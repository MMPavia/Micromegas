#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Frame_incollatrice/"
#define outpath "/home/atlas/Micromegas/Simone/Output/"

void Graph_largh_frame_incollatrice (TString scan, TString scan1){

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
	
	cout << "starting reading file " << endl;
        int j=0;

	// looping on the file lines
	while (1) {
		
		in >> nmis >> u  >> u  >> u;
		
		if ( !in.good() || in.eof()) break;  // another sanity check

	

        
       		in >> s[j];
       		x[j]= (nmis/10);
		s[j]= (s[j]+ 26.005);

       		cout <<"x= " << x[j] << endl;
    		cout <<"s= " << s[j] << endl;
		cout <<"nmis " << nmis << endl;

         
                in >> date >> time; 
                cout << "date " << date <<" time " << time << endl;
		j++;
		
	} // end while


                cout <<"x= " << x[1] << endl;
                cout <<"s= " << s[1] << endl;
                cout <<"nmis " << nmis << endl;
                cout <<"x= " << x[j-1] << endl;
                cout <<"s= " << s[j-1] << endl;



	cout << "file completed "<< endl;


	// close the input file
	in.close();


        ostringstream file2;
        file2 << inpath  << scan1 ;




	//sanity check

        ifstream in1(file2.str().c_str());

         if (!in1 || in1.bad() )
         {
                cout << "file non letto" << endl;
                return; // sanity check
        }

        cout << "starting reading file " << endl;
        j=0;


        while (1) {

                in1 >> nmis >> u  >> u  >> u ;

                if ( !in1.good() || in1.eof()) break;  // another sanity check




                in1 >> t[j];
                x[j]=(nmis/10);
                t[j]= (t[j]+ 26.005);

                cout <<"x= " << x[j] << endl;
                cout <<"s= " << t[j] << endl;
                cout <<"nmis " << nmis << endl;


                in1 >> date >> time;
                cout << "date " << date <<" time " << time << endl;
                j++;

        }

        in1.close();





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


  return;
}
