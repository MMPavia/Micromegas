#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Modulo_0/LinearH/Prime_misure_modulo_0/Frame_modulo_0/"
#define outpath "/home/atlas/Micromegas/Simone/Output/Modulo_0/LinearH/Mappe_frame/Mappe_9_frame/"

void Mappe_accettata_mediata_2_lati (string scan1, string scan2){
// style option
// gROOT->LoadMacro("AtlasUtils.C");   // myText
//SetAtlasStyle(); 
 gStyle->SetPalette(1);
 gStyle->SetOptStat(111111);
 gStyle->SetPaintTextFormat(".0f");
// gStyle->SetOptTitle(0);
 gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
 gStyle->SetNumberContours(100);

// file containing the data
   ostringstream file1;
   file1 << inpath  << scan1 ;  
   ostringstream file2;
   file2 << inpath  << scan2 ;


// histograms binning info
  int nxbin=10;
  int nybin=3;
  float xmin = 0;
  float xmax = 10;
  float ymin = 1;
  float ymax =4;

//  2d map of the table surface
   TH2F *tmap1= new TH2F("Frame 209","Frame number 116", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap1->SetTitleOffset(2);
   tmap1->GetXaxis()->SetTitle("X (cm)");
   tmap1->GetZaxis()->SetRangeUser(-70,70);
   tmap1->GetYaxis()->SetTitle("Y (x)");
   tmap1->GetZaxis()->SetTitle("Z (#mum)");

   cout << "histos created " << endl;

   Float_t x,y;
   Double_t z1,z2, coord, a, u;
   Int_t nmis1,nmis2,j;
   string date, time;


// Prima frame
	
   ifstream in1(file1.str().c_str());
   ifstream in2(file2.str().c_str());
   if (!in1 || in1.bad() )
   {
	cout << "file 1 non letto" << endl;
	return; 
   }

   if (!in2 || in2.bad() )
   {
        cout << "file 2 non letto" << endl;
        return; 
   }
   cout << "starting reading file " << endl;


   while (1) {
		
	in1 >> nmis1;
	in2 >> nmis2;
	
	if ( !in1.good() || in1.eof()) break; 
        if ( !in2.good() || in2.eof()) break;

	cout <<"nmis " << nmis1 << endl;
		
		in1 >> z1;
		in2 >> z2;

		coord=(((z1+z2)/2)-10.000)*1000;
		x= nmis1-1;
		y= 1;
		tmap1->Fill(x, y, coord);
        
                in1 >> z1;
                in2 >> z2;
                
                coord= (((z1+z2)/2)-10.000)*1000;
                y= 2;
                tmap1->Fill(x, y, coord);

                in1 >> z1;
                in2 >> z2;
                
                coord= (((z1+z2)/2)-10.000)*1000;
                y= 3;
                tmap1->Fill(x, y, coord);		
	
	cout << "x= " << x << endl;	
	cout << "nmis= " << nmis1 << endl;
	cout <<"coord= " <<coord << endl;	
	in1 >> date >> time;
	in2 >> date >> time;
 
	cout << "date " << date <<" time " << time << endl; 

   } 

   cout << "file completed "<< endl;

   in1.close();
   in2.close();


/*
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
   rootfile  << outpath <<"myout_"<<scan1 <<".root";
   TFile myroot(rootfile.str().c_str(), "RECREATE");
   cout << "Root output file: " << endl << rootfile.str() << endl;

*/




   ostringstream Smap;
   Smap << outpath << "/map_"<< scan1 <<".png";



    TCanvas *c0 = new TCanvas("c0", "c0", 1000,700);
    tmap1->SetMarkerSize(0.8);
    tmap1->Draw("colz,text45");
    tmap1->SetStats(kFALSE);
    tmap1->Write();

    c0->Print(Smap.str().c_str());
	
 
	//myroot.Close();


  return;
}
