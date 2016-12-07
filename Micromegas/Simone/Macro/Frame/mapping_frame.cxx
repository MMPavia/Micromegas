#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Frame_3_punti/"
#define outpath "/home/atlas/Micromegas/Simone/Output/"

void mapping_frame (string scan){

// style option
 gROOT->LoadMacro("AtlasUtils.C");   // myText
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
	
// create all the histograms
/*
   TH1F *frame1 = new TH1F("punto1","punto1", 20, 9.90, 10.10 );
   TH1F *frame2 = new TH1F("punto2","punto2", 20, 9.90, 10.10 );
   TH1F *frame3 = new TH1F("punto3","punto3", 20, 9.90, 10.10 );
*/

//  2d map of the table surface
   TH2F *tmap= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap->GetXaxis()->SetTitle("X (cm)");
   tmap->GetYaxis()->SetTitle("Y (x)");
   tmap->GetZaxis()->SetTitle("Z (#mum)");

   cout << "histos created " << endl;

//variables for reading the file
	
   Float_t x,y;
   Double_t z, coord;
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
		
	in >> nmis;
		
	if ( !in.good() || in.eof()) break;  // another sanity check

	cout <<"nmis " << nmis << endl;
		

	for (j=1;j<4;j++)
	{		
		
		in >> z ;
		y=j;
		x=(nmis-1)*10;
		coord= ( z-10.00)*1000;

	        cout <<"x= " << x << endl;
                cout <<"y= " << y << endl;
                cout <<"z= " << z << endl;
                cout <<"coord= " << coord << endl;
	
		tmap->Fill(x,y,coord);
        


	}
		
		
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


//  controllo per buchi

   Int_t binx = tmap->GetNbinsX();
   Int_t biny = tmap->GetNbinsY();
   cout << "numero di bin in y= " << biny << endl;
   cout << "numero di bin in x= " << binx << endl;


   for (int i=1; i<=biny; i++){
   	for (int j=1; j<=binx; j++){

	        cout << " i " << i << " j " << j << endl;
        	double t = tmap->GetBinContent(j,i);
		cout << "coord = " << t << endl;


/*
  	 if (t<9.8) {
  		 double b = tmap->GetBinContent(j,i-2);
  		 // double c = tmap->GetBinContent(j+1,i-1);
 	 	 // double d = tmap->GetBinContent(j+1,i);
  		 // double e = tmap->GetBinContent(j+1,i+1);
  		 double f = tmap->GetBinContent(j,i+2);
 	 	 double mean = (b+f)/2;
  		 tmap->SetBinContent(j,i,mean);
  	 };
*/
    
     }
   }
	
	
	// create a root file for the histograms; 
   ostringstream rootfile;
   rootfile  << outpath <<"myout_"<<scan <<".root";
   TFile myroot(rootfile.str().c_str(), "RECREATE");
   cout << "Root output file: " << endl << rootfile.str() << endl;

   ostringstream Smap;
   Smap << outpath << "/map_"<< scan <<".png";


/*	// create a canvas for each plot and drawing with different options
	TCanvas *c0 = new TCanvas();
	tmap->Draw();
	tmap->Write();
*/

    TCanvas *c0 = new TCanvas("c0", "c0", 1000,700);
    tmap->SetMarkerSize(0.8);
    tmap->Draw("colz,text45");
    tmap->SetStats(kFALSE);
    tmap->Write();
    c0->Print(Smap.str().c_str());
	
 
	//myroot.Close();


  return;
}
