#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
//#include "AtlasStyle.C"


//macro che ha i root file che arrivano da repeat&mean 
//e vuole in input i txt del run sui pcb

#define inpath1 "/home/atlas/Micromegas/ProdData/mapping/"
#define inpath2 "/home/atlas/Micromegas/ProdData/reference/"
#define outpath "/home/atlas/Micromegas/ProdData/root_plot/"

void PILLAR_diff_pcb_ref_stiffback (string scan){

  // style option
   gROOT->LoadMacro("AtlasUtils.C");   // myText
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".0f");
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


// file containing the data.txt
   ostringstream file1;
   file1 << inpath1 << scan;
   scan.erase(scan.end()-4,scan.end());
   scan.erase(scan.begin(),scan.begin()+13);

// reference files
   ostringstream file2;
   file2 << inpath2 << "reference_stiffback_pillar.root"; 

   TFile *f1 = new TFile(file2.str().c_str());
   TH2F *ref_s = (TH2F*)f1->Get("map_s1");
   TH2F *ave_s = (TH2F*)ref_s->Clone("reference");
   ave_s->SetTitle("reference Map");

   int nxbin=24;   // 
   int nybin=30;   //
   float xmin = 320/10;
   float xmax = 2440/10;
   float ymax = 1230;
   float ymin = 200;
    
   TH2F *map_s = new TH2F("map_s","map_s", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s->GetXaxis()->SetTitle("X (cm)");
   map_s->GetYaxis()->SetTitle("Y (mm)");
   map_s->GetZaxis()->SetTitle("Z (#mum)");

   TH2F *diff = new TH2F("diff","diff", nxbin, xmin, xmax, nybin, ymin, ymax );
   diff->GetXaxis()->SetTitle("X (cm)");
   diff->GetYaxis()->SetTitle("Y (mm)");
   diff->GetZaxis()->SetTitle("Z (#mum)");

   int ndistbin = 100; 
   float xdistmin =600; //pcb
   float xdistmax =900; //pcb

   //TH1F *distz_s = new TH1F("distz_s","distz_s", ndistbin,xdistmin, xdistmax  ); 
   //distz_s->GetXaxis()->SetTitle("#mum ");
   //distz_s->GetYaxis()->SetTitle("counts");
   //distz_s->SetFillColor(2);

   TH1F *distz_diff = new TH1F("distz_diff","distz_diff",  ndistbin,xdistmin, xdistmax  );
   distz_diff->GetXaxis()->SetTitle("#mum ");
   distz_diff->GetYaxis()->SetTitle("counts");
   distz_diff->SetFillColor(2);
   

   Int_t j=0; 

   Float_t x,y, opt, laser, temp1, temp2, coord;

// leggo file e riempio array con coord

    ifstream in1(file1.str().c_str());
    if (!in1 || in1.bad()) return;

    while(1){
      
       in1 >> x >> y >> opt >> laser >>  temp1 >> temp2;
	   if(!in1.good()) break; 
	   coord = (laser - opt)*1000;
	   cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " 
		<< temp2 << "c1=  " << coord << " "  << endl;
	   //distz_s->Fill(coord);
	   map_s->Fill(x/10,-y, coord);
	   j++;
    }

   diff->Add(ref_s,map_s,-1,+1);

   float z; 
    
   for (int i=1; i<=nybin; i++){
      for (int j=1; j<=nxbin; j++){
	  z = diff->GetBinContent(j,i);
	  if(z!=0) {
	     distz_diff->Fill(z);
	     //cout << " z=  " << z << endl;
          }
      }
    }


 // create a root file for the histograms;
  ostringstream rootfile;
  rootfile  << outpath <<"/diff_pcb_stiffbackref_" << scan << ".root"; 
  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;


   TCanvas *c0 = new TCanvas("map_s","map_s",1000,700);
   map_s->SetMarkerSize(0.8);
   map_s->Draw("colz1,text45");
   map_s->SetStats(kFALSE);
   map_s->GetZaxis()->SetRangeUser(500, 800);
   map_s->Write();

   TCanvas *c2 = new TCanvas("ref_s","ref_s",1000,700);
   ref_s->SetMarkerSize(0.8);
   ref_s->Draw("colz1,text45");
   ref_s->SetStats(kFALSE);
   ref_s->Write();
   
   TCanvas *c23 = new TCanvas("diff_distrib","diff_distrib",1000,700);
   distz_diff->Draw();
   distz_diff->Write();   
   
   TCanvas *c4 = new TCanvas("diff_map","diff_map",1000,700);
   diff->SetMarkerSize(0.8);
   diff->Draw("colz1,text45");
   diff->SetStats(kFALSE);
   diff->GetZaxis()->SetRangeUser(500, 800);
   diff->Write();


  myroot.Close();

   return; 
}


