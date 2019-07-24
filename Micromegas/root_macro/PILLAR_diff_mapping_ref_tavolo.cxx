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

//P=0,1,2 (ref/pcb/pannello)

void PILLAR_diff_mapping_ref_tavolo (int p, string scan){

  // style option
   gROOT->LoadMacro("AtlasUtils.C");   // myText
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   if (p==2) {
	gStyle->SetPaintTextFormat(".3f");
	gStyle->SetStatFormat("6.5g"); 
   }
   else gStyle->SetPaintTextFormat(".0f");
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


// file containing the data.txt
   ostringstream file1;
   file1 << inpath1 << scan;
   scan.erase(scan.end()-4,scan.end());
   scan.erase(scan.begin(),scan.begin()+13);

// reference files
   ostringstream file2;
   file2 << inpath2 << "reference_table_pillar.root"; 

   TFile *f1 = new TFile(file2.str().c_str());
   TH2F *ref_s = (TH2F*)f1->Get("mean_map");
   TH2F *ave_s = (TH2F*)ref_s->Clone("average");
   ave_s->SetTitle("Average Reference Map");
   ave_s->Scale(0.001);



   int nxbin=24;   // 
   int nybin=30;   //
   float xmin = 320/10;
   float xmax = 2440/10;
   float ymax = 1230;
   float ymin = 200;
    
   TH2F *map_s = new TH2F("map_s","map_s", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s->GetXaxis()->SetTitle("X (cm)");
   map_s->GetYaxis()->SetTitle("Y (mm)");
   if (p==2) map_s->GetZaxis()->SetTitle("Z (mm)");
   else map_s->GetZaxis()->SetTitle("Z (#mum)");

   TH2F *diff = new TH2F("diff","diff", nxbin, xmin, xmax, nybin, ymin, ymax );
   diff->GetXaxis()->SetTitle("X (cm)");
   diff->GetYaxis()->SetTitle("Y (mm)");
   if (p==2) diff->GetZaxis()->SetTitle("Z (mm)");
   else diff->GetZaxis()->SetTitle("Z (#mum)");
   

   float xdistmin ; 
   float xdistmax ; 
   int ndistbin = 100; 
  
   if(p==0){
     xdistmin = -100; 
     xdistmax = 100;
   }
   else if (p==1){ 
      xdistmin =600; //pcb
      xdistmax =900; //pcb
   }
   else if (p==2){
      xdistmin =11.5; //panel
      xdistmax =11.9; //panel
   }
   else{
     cout << "wrong value for p " << endl;
     return;
   }


   TH1F *distz_s = new TH1F("distz_s","distz_s", ndistbin,xdistmin, xdistmax  ); 
   if (p==2) distz_s->GetXaxis()->SetTitle("height (mm) ");
   else distz_s->GetXaxis()->SetTitle("height (#mum) ");
   distz_s->GetYaxis()->SetTitle("counts");
   distz_s->SetFillColor(2);

   TH1F *distz_diff = new TH1F("distz_diff","distz_diff",  ndistbin,xdistmin, xdistmax  );
   if (p==2) distz_diff->GetXaxis()->SetTitle("height (mm) ");
   else distz_diff->GetXaxis()->SetTitle("height (#mum) ");
   distz_diff->GetYaxis()->SetTitle("counts");
   distz_diff->SetFillColor(2);
   
   TH1F *distz_pillar = new TH1F("distz_pillar","distz_pillar",  ndistbin,xdistmin, xdistmax  );
   if (p==2) distz_pillar->GetXaxis()->SetTitle("height (mm) ");
   else distz_pillar->GetXaxis()->SetTitle("height (#mum) ");
   distz_pillar->GetYaxis()->SetTitle("counts");
   distz_pillar->SetFillColor(2);

   Int_t j=0; 

   Float_t x,y, opt, laser, temp1, temp2, coord;
   Double_t media,stdev, RMS;
   Int_t q=0, n=146;
   Float_t array[n];
   Double_t minimo=14000;
   Double_t massimo=-14000;



// leggo file e riempio array con coord

    ifstream in1(file1.str().c_str());
    if (!in1 || in1.bad()) return;

    while(1){
	in1 >> x >> y >> opt >> laser >>  temp1 >> temp2;
	if(!in1.good()) break; 
	if(p==2) coord = (laser - opt);
	else coord = (laser - opt)*1000;
	cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " << temp2 << "c1=  " 
	     << coord << " "  << endl;
	distz_s->Fill(coord);
	map_s->Fill(x/10,-y, coord);
	j++;
    }

   if(p==2) diff->Add(ave_s,map_s,-1,+1);
   else diff->Add(ref_s,map_s,-1,+1);

   float z, zpillar; 
    
   for (int i=1; i<=nybin; i++){
      for (int j=1; j<=nxbin; j++){
	  z = diff->GetBinContent(j,i);
	  zpillar = z+ 0.128;
	  if(z!=0) {
	  array[j]=z;
	  q += array[j];
	  if(array[j]<minimo){minimo = array[j];}
          if(array[j]>massimo){massimo = array[j];}

	  distz_diff->Fill(z);
	  distz_pillar->Fill(zpillar);
          }
      }
    }

   media =  distz_diff->GetMean();
   stdev =  distz_diff->GetRMS();

   cout << " media= " << media << " rms= " << stdev << " min= " << minimo << "max= " << massimo << endl;

 // create a root file for the histograms;
  ostringstream rootfile;
  rootfile  << outpath <<"/diff_pan_plate_" << scan << ".root"; 
  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;

  // output data file
  ofstream Data;
  ostringstream txtfile;
  txtfile << outpath <<"/summary_diff_pan_plate_" << scan << ".txt"; 
  Data.open(txtfile.str().c_str(), ofstream::out | ofstream::app);
  cout << "Text output file: " << endl << txtfile.str() << endl;
  Data << "Mean   rms   min   max" << "\n" << media << "\t" << stdev << "\t" << minimo << "\t" << massimo << endl;


   TCanvas *c0 = new TCanvas("map_s","map_s",1000,700);
   map_s->SetMarkerSize(0.8);
   map_s->Draw("colz1,text45");
   map_s->SetStats(kFALSE);
   if (p==1) map_s->GetZaxis()->SetRangeUser(600, 800);
   if (p==2) {
	map_s->GetZaxis()->SetRangeUser(11.5, 11.88);
   	map_s->GetZaxis()->SetLabelSize(0.04);
   }    
   map_s->Write();

   TCanvas *c2 = new TCanvas("ref_s","ref_s",1000,700);
   ref_s->SetMarkerSize(0.8);
   ref_s->Draw("colz1,text45");
   ref_s->SetStats(kFALSE);
   ref_s->Write();
   
   TCanvas *c4 = new TCanvas("diff_map","diff_map",1000,700);
   diff->SetMarkerSize(0.8);
   diff->Draw("colz1,text45");
   diff->SetStats(kFALSE);
   if (p==1) diff->GetZaxis()->SetRangeUser(600, 800);
   if (p==2) {
	diff->GetZaxis()->SetRangeUser(11.5, 11.88);
   	diff->GetZaxis()->SetLabelSize(0.04);
   }
   diff->Write();
   
   media=distz_diff->GetMean(1);
   RMS=distz_diff->GetRMS(1);
   Data << media  << " \t " << RMS << endl;


   TCanvas *c23 = new TCanvas("diff_distrib","diff_distrib",1000,700);
   distz_diff->Draw();
   distz_diff->Write();   

   if(p==2){

  	 TCanvas *c24 = new TCanvas("diff+pillar_distrib","diff+pillar_distrib",1000,700);
    	 distz_pillar->Draw();
   	 distz_pillar->Write();
   }


   myroot.Close();

   return; 
}





















