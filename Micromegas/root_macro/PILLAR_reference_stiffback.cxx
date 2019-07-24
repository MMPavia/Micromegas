#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
//#include "AtlasStyle.C"

#define inpath "/home/atlas/Micromegas/ProdData/mapping/"
#define outpath "/home/atlas/Micromegas/ProdData/reference/"


void PILLAR_reference_stiffback (string scan1){


  // style option
   gROOT->LoadMacro("AtlasUtils.C");   // myText
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".0f");
   //gStyle->SetOptTitle(0);
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


// file containing the data
   ostringstream file1;
   file1 << inpath  << scan1;
   scan1.erase(scan1.end()-4,scan1.end());
   scan1.erase(scan1.begin(),scan1.begin()+13);
   string nomefile=file1.str();
 
   int nxbin=24;   // 
   int nybin=30;   //
   float xmin = 320/10;
   float xmax = 2440/10;
   float ymax = 1230;
   float ymin = 200;
   
   TH2F *map_s1 = new TH2F("map_s1","map_s1", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s1->GetXaxis()->SetTitle("X (cm)");
   map_s1->GetYaxis()->SetTitle("Y (mm)");
   map_s1->GetZaxis()->SetTitle("Z (#mu m)");



   float xdistmin, xdistmax;
   int ndistbin = 100;
   xdistmin = -250; 
   xdistmax = 50; 


   TH1F *distz_s1 = new TH1F("distz_s1","distz_s1", ndistbin,xdistmin, xdistmax  ); 
   distz_s1->GetXaxis()->SetTitle("#mu ");
   distz_s1->GetYaxis()->SetTitle("counts");
   distz_s1->SetFillColor(2);


   Float_t x,y, opt, laser, press, temp1,coord;
   Double_t media,stdev,minimo,massimo;

// leggo file e riempio array con coord

    ifstream in1(file1.str().c_str());
    if (!in1 || in1.bad()) return; // sanity check

    int j=0; 

    while (1) {
	in1 >> x >> y >> opt >> laser >> press >>  temp1;
        if (!in1.good()) break;
   
	cout << x << " " << y << " " <<  opt << " " <<  laser << " " << press << " " <<  temp1 << endl;;
        coord = (laser - opt)*1000;
        map_s1->Fill(x/10,-y,coord);   
	distz_s1->Fill(coord);

	j++;

    }

    cout << "nlines " << j << endl;  

    in1.close(); 


 // create a root file for the histograms;
  ostringstream rootfile;
  rootfile  << outpath <<"reference_stiffback_pillar.root";

  
  //create a txt data file;
  ofstream Data;
  ostringstream txtfile;
  txtfile << outpath <<"/summary_" << scan1 << ".txt"; 


  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;

   TCanvas *c21 = new TCanvas("distrib","distrib",1000,700);
   distz_s1->Draw();
   distz_s1->Write();

   TCanvas *c0 = new TCanvas("map","map",1000,700);
   map_s1->SetMarkerSize(0.8);
   map_s1->Draw("colz1,text45");
   map_s1->SetStats(kFALSE);
   map_s1->Write();


   
   myroot.Close();

   return; 
}





















