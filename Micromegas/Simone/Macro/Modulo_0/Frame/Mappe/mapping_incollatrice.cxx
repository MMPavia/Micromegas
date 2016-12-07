#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Spessore_frame/"
#define outpath "/home/atlas/Micromegas/Simone/Output/Larghezza_frame/Spessore_frame_tutti/"

void mapping_incollatrice (string scan){

// style option
// gROOT->LoadMacro("AtlasUtils.C");   // myText
//SetAtlasStyle(); 
 gStyle->SetPalette(1);
 gStyle->SetOptStat(111111);
 gStyle->SetPaintTextFormat(".0f");
 gStyle->SetOptTitle(0);
 gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
 gStyle->SetNumberContours(100);

 
	
// file containing the data
   ostringstream file1;
   file1 << inpath  << scan ;  

// histograms binning info
  int nxbin=12;
  int nybin=3;
  float xmin = 0;
  float xmax = 120;
  float ymin = 1;
  float ymax =4;
	

//  2d map of the table surface
   TH2F *tmap= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap->GetXaxis()->SetTitle("X (cm)");
   tmap->GetYaxis()->SetTitle("Y (x)");
   tmap->GetZaxis()->SetTitle("Z (#mum)");

   cout << "histos created " << endl;

//variables for reading the file
	
   Float_t x,y;
   Double_t z, coord, a, u;
   Int_t nmis,j;
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
		
	in >> nmis>>a>>u>>u;
		
	if ( !in.good() || in.eof()) break;  // another sanity check

	cout <<"nmis " << nmis << endl;
		

		
		in >> z ;
		
		if(a==0){
		y=1;
		}

                if(a==6){
                y=2;
                }

                if(a==12){
                y=3;
                }

		x=(nmis/10);
		coord= (( z + 9.995) - 10.000)*1000;

	        cout <<"x= " << x << endl;
                cout <<"y= " << y << endl;
                cout <<"z= " << z << endl;
                cout <<"coord= " << coord << endl;
	
		tmap->Fill(x, y, coord);
        
		
		
	in >> date >> time; 
	cout << "date " << date <<" time " << time << endl; 
                cout <<"z= " << z << endl;
                cout <<"coord= " << coord << endl;

		
   } // end while



   cout << "file completed "<< endl;

   // close the input file
   in.close();



//  controllo per buchi

   Int_t binx = tmap->GetNbinsX();
   Int_t biny = tmap->GetNbinsY();
   cout << "numero di bin in y= " << biny << endl;
   cout << "numero di bin in x= " << binx << endl;


   for (int i=1; i<=biny; i++){
   	for (int j=1; j<=binx; j++){

	        cout << " i= " << i << endl << " j= " << j << endl;
        	double t = tmap->GetBinContent(j,i);
		cout << "coord = " << t << endl;

    
     }
   }
	
	
	// create a root file for the histograms; 
   ostringstream rootfile;
   rootfile  << outpath <<"myout_"<<scan <<".root";
   TFile myroot(rootfile.str().c_str(), "RECREATE");
   cout << "Root output file: " << endl << rootfile.str() << endl;

   ostringstream Smap;
   Smap << outpath << "/map_"<< scan <<".png";



    TCanvas *c0 = new TCanvas("c0", "c0", 1000,700);
    tmap->SetMarkerSize(0.8);
    tmap->Draw("colz,text45");
    tmap->SetStats(kFALSE);
    tmap->Write();
    c0->Print(Smap.str().c_str());
	
 
	//myroot.Close();


  return;
}
