#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
//#include "AtlasStyle.C"

#define inpath1 "/home/atlas/Micromegas/ProdData/mapping/"
#define inpath2 "/home/atlas/Micromegas/ProdData/reference/"
#define outpath "/home/atlas/Micromegas/ProdData/root_plot/"

//p=1,2,3,4 -> int sx, int dx, ext sx, ext dx

void DIAG_diff_panel_ref_tavolo (int p, string scan1){

  // style option
   gROOT->LoadMacro("AtlasUtils.C");   // myText
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".3f");
   gStyle->SetStatFormat("6.5g");
   //gStyle->SetOptTitle(0);
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


// file containing the data
   ostringstream file1;
   file1 << inpath1  << scan1;
   scan1.erase(scan1.end()-4,scan1.end());
   scan1.erase(scan1.begin(),scan1.begin()+13);

// reference files
   ostringstream file2;

     if(p==1){
       file2 << inpath2 << "reference_table_diag_int_sx.root";
     }
     else if(p==2){
       file2 << inpath2 << "reference_table_diag_int_dx.root";
     }
     else if(p==3){
       file2 << inpath2 << "reference_table_diag_ext_sx.root";
     }
     else if(p==4){
      file2 << inpath2 << "reference_table_diag_ext_dx.root";
     }


   TFile *f1 = new TFile(file2.str().c_str());
   TH2F *ref_s1 = (TH2F*)f1->Get("mean_map");
   TH2F *ave_s1 = (TH2F*)ref_s1->Clone("average");
   ave_s1->SetTitle("Average Map S1");
   ave_s1->Scale(0.001);

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


   
   TH2F *map = new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   map->GetXaxis()->SetTitle("X (cm)");
   map->GetYaxis()->SetTitle("Y (mm)");
   map->GetZaxis()->SetTitle("Z (#mum)");


   TH2F *diff1 = new TH2F("diff1","diff1", nxbin, xmin, xmax, nybin, ymin, ymax );
   diff1->GetXaxis()->SetTitle("X (cm)");
   diff1->GetYaxis()->SetTitle("Y (mm)");
   diff1->GetZaxis()->SetTitle("Z (#mum)");


 
   int ndistbin = 100; //cambiare se sono PCB o pannello
   float xdistmin = 11.3; 
   float xdistmax = 11.6; 


   TH1F *distz_diff = new TH1F("distz_diff","distz_diff",  ndistbin,xdistmin, xdistmax  );
   distz_diff->GetXaxis()->SetTitle("height (mm) ");
   distz_diff->GetYaxis()->SetTitle("counts");
   distz_diff->SetFillColor(2);
      
   Int_t j=0; 
   Float_t x,y, opt, laser, temp1, temp2, coord;



    ifstream in1(file1.str().c_str());
    if (!in1 || in1.bad()) return;

    while(1){
      
           in1 >> x >> y >> opt >> laser >>  temp1 >> temp2;
	   if(!in1.good()) break; 

	   coord = (laser - opt);
	   cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " << temp2 << "c1=  " << coord << " "  << endl;
           map->Fill(x/10,-y, coord);
           //map_p->Fill(x/10,-y, coord-11.80); // 11.80 spessore pannello
	   j++;
    }

    
    diff1->Add(ave_s1,map,-1,+1);


    float z1 ; 
    
   for (int i=1; i<=nybin; i++){
      for (int j=1; j<=nxbin; j++){
	  z1 = diff1->GetBinContent(j,i);
	  if(z1!=0) {
	     distz_diff->Fill(z1);
	     cout <<  " z1=  " << z1 << endl;
          }
         }
      }



 // create a root file for the histograms;
  ostringstream rootfile;
  rootfile  << outpath <<"/diff_pcb_plate_diag_" << scan1  << ".root";   //*************!!!!!!
  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;


   TCanvas *c0 = new TCanvas("map","map",1000,700);
   map->SetMarkerSize(0.8);
   map->Draw("colz1,text45");
   map->SetStats(kFALSE);
   map->GetZaxis()->SetRangeUser(11.3, 11.7);
   map->Write();


   TCanvas *c2 = new TCanvas("ref","ref",1000,700);
   ref_s1->SetMarkerSize(0.8);
   ref_s1->Draw("colz1,text45");
   ref_s1->SetStats(kFALSE);
   ref_s1->Write();


   TCanvas *c4 = new TCanvas("diff_map","diff_map",1000,700);
   diff1->SetMarkerSize(0.8);
   diff1->Draw("colz1,text45");
   diff1->SetStats(kFALSE);
   diff1->GetZaxis()->SetRangeUser(11.3, 11.6);
   diff1->Write();


   TCanvas *c23 = new TCanvas("diff_distrib","diff_distrib",1000,700);
   distz_diff->Draw();
   distz_diff->Write();   


  myroot.Close();

   return; 
}





















