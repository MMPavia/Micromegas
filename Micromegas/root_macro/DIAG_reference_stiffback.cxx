#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
//#include "AtlasStyle.C"

#define inpath "/home/atlas/Micromegas/ProdData/mapping/"
#define outpath "/home/atlas/Micromegas/ProdData/reference/"



//p=1,2,3,4 -> int sx, int dx, ext sx, ext dx

void DIAG_reference_stiffback (int p, string scan1){

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
   float xmin, xmax, ymin, ymax;
   if (p==1) {
        xmin = 300/10;    //frame sx (riferimento da pc)
        xmax = 2450/10;
        ymax = 550;
        ymin = 150;
   }
   else if (p==2) {
        xmin = 300/10;   //frame dx
        xmax = 2450/10;
        ymax = 1280;
        ymin = 870;
   }
   else if (p==3) {
        xmin = 325/10;   //mask sx
        xmax = 2390/10;
        ymax = 465;
        ymin = 78;
   }
   else if (p==4) {
        xmin = 325/10; //mask dx
        xmax = 2390/10;
        ymax = 1360;
        ymin = 960;
   }

   
   TH2F *map_s1 = new TH2F("map_s1","map_s1", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s1->GetXaxis()->SetTitle("X (cm)");
   map_s1->GetYaxis()->SetTitle("Y (mm)");
   map_s1->GetZaxis()->SetTitle("Z (#mu m)");


   int ndistbin = 50; 
   float xdistmin = 20; 
   float xdistmax = -100; 


   TH1F *distz_s1 = new TH1F("distz_s1","distz_s1", ndistbin,xdistmin, xdistmax  ); 
   distz_s1->GetXaxis()->SetTitle("#mu ");
   distz_s1->GetYaxis()->SetTitle("counts");
   distz_s1->SetFillColor(2);
   

   Float_t scan[7][266]={0};
   Float_t x,y, opt, laser, press, temp2, coord;


    ifstream in1(file1.str().c_str());
    if (!in1 || in1.bad()) return;

    while(1){
      
           in1 >> x >> y >> opt >> laser >>  press >> temp2;
	   if(!in1.good()) break; 

	   coord = (laser - opt)*1000;
	   map_s1->Fill(x/10,-y,coord);
           distz_s1->Fill(coord);
    
	}


      in1.close();



 // create a root file for the histograms;
  ostringstream rootfile;
     if(p==1){
       rootfile << outpath << "reference_stiffback_diag_int_sx.root";
     }
     else if(p==2){
       rootfile << outpath << "reference_stiffback_diag_int_dx.root";
     }
     else if(p==3){
       rootfile << outpath << "reference_stiffback_diag_ext_sx.root";
     }
     else if(p==4){
       rootfile << outpath << "reference_stiffback_diag_ext_dx.root";
     }
  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;

   TCanvas *c21 = new TCanvas("c21","c21",1000,700);
   distz_s1->Draw();
   distz_s1->Write();
   
   TCanvas *c0 = new TCanvas("c0","c0",1000,700);
   map_s1->SetMarkerSize(0.8);
   map_s1->Draw("colz1,text45");
   map_s1->SetStats(kFALSE);
   map_s1->Write();



  myroot.Close();

   return; 
}





















