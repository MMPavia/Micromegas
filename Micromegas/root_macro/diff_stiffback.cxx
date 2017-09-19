#include <stdio.h>
#include <string>
#include <vector>
//#include "AtlasStyle.C"

#define path "/home/atlas/Micromegas/ProdData/root_plot/stiffback/"

void diff_stiffback (string scan1, string scan2){

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
  ostringstream infile1;
  infile1 << path  << scan1 ;
  scan1.erase(scan1.end()-5,scan1.end());
  ostringstream infile2;
  infile2 << path  << scan2 ;  
  scan2.erase(scan2.end()-5,scan2.end());


   TFile *s1 = new TFile(infile1.str().c_str());
   TH2F *map1 = (TH2F*)s1->Get("map_wrt_mean"); 


   TFile *s2 = new TFile(infile2.str().c_str());
   TH2F *map2 = (TH2F*)s2->Get("map_wrt_mean"); 

   cout << "diff1"<<endl;
   Int_t binx = map1->GetNbinsX();
   Int_t biny = map2->GetNbinsY();
   cout << binx << " " << biny << endl;

  int nxbin=5;
  int nybin=4;
  float xmin = 225;
  float xmax = 2500;
  float ymin = 0;
  float ymax = 1330;

   TH2F *Map_diff= new TH2F("Map_diff","Map_diff", binx, xmin, xmax, biny, ymin, ymax );
   Map_diff->GetXaxis()->SetTitle("X (mm)");
   Map_diff->GetYaxis()->SetTitle("Y (mm)");
   Map_diff->GetZaxis()->SetTitle("Z (#mum)");


   Map_diff->Add(map1,map2,-1,+1);


   ostringstream rootfile;
   rootfile  << path << "diff_stiffback_" << scan1 << "_"<< scan2 << ".root";
   ostringstream nome;
   nome << path << "/diff_stiffback_" << scan1 << "_"<< scan2 << ".png";
   TFile myroot(rootfile.str().c_str(), "RECREATE");
   cout << "Root output file: " << endl << rootfile.str() << endl;

   TCanvas *c00 = new TCanvas("c00", "c00", 1000,700); 
   TPad *pad2 = new TPad("pad2","The pad with the histogram",0.03,0.02,0.97,0.9,0);
   pad2->Draw();
   TPaveLabel *title2 = new TPaveLabel(0.12,0.91,0.32,0.96,"Maps Difference");
   title2->SetFillColor(kBlue-10);
   title2->SetTextFont(42);
   title2->Draw();
   pad2->cd();
   Map_diff->Draw("COLZ1TEXT45");
   Map_diff->SetStats(kFALSE); 
  // myText(0.75,0.75,1,"Vacuum ON"); 
   Map_diff->Write();
   c00->Print(nome.str().c_str());

   myroot.Close(); 


  
   return; 
}
