#include <stdio.h>
#include <string>
#include <vector>
//#include "AtlasStyle.C"

#define path "/home/atlas/Micromegas/M0Data/root_plot/"

void diff_pcb_piastre (TString scan){

  // style option
   gROOT->LoadMacro("AtlasUtils.C");   // myText
   //SetAtlasStyle(); 
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".0f"); 
   gStyle->SetOptTitle(0);
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


// apro i file
   ostringstream infile1;
   //infile1 << path << "ReferenceTablePlate.root"; 
   infile1 << path << "ReferenceTableFebruary.root"; 

   ostringstream infile2;
   infile2 << path  << "/pcb/"<< scan ;

   TFile *s1 = new TFile(infile1.str().c_str());
   TH2F *map1 = (TH2F*)s1->Get("mean_map"); 
   TH2F *ave = (TH2F*)map1->Clone("averagee");
   ave->SetTitle("Average Map");
   //ave->Scale(0.001);

   TFile *s2 = new TFile(infile2.str().c_str());
   TH2F *map2 = (TH2F*)s2->Get("map"); 


   Int_t binx = ave->GetNbinsX();
   Int_t biny = ave->GetNbinsY();
   cout << binx << " " << biny << endl;

   float xmin = -40;
   float xmax = 2280;
   float ymin = -40;
   float ymax =1320;
   float xstep = (xmax-xmin)/binx;
   float ystep = (ymax-ymin)/biny;

  
   TH2F *Map_diff= new TH2F("Map_diff","Map_diff", binx, xmin, xmax, biny, ymin, ymax );
   Map_diff->GetXaxis()->SetTitle("X (mm)");
   Map_diff->GetYaxis()->SetTitle("Y (mm)");
   Map_diff->GetZaxis()->SetTitle("Z (#mum)");
   
   TH2F *Map_diff_B= new TH2F("Map_diff_B","Map_diff_B", binx, xmin, xmax, biny, ymin, ymax );
   Map_diff_B->GetXaxis()->SetTitle("X (mm)");
   Map_diff_B->GetYaxis()->SetTitle("Y (mm)");
   Map_diff_B->GetZaxis()->SetTitle("Z (#mum)");

   TH2F *diff_B= new TH2F("diff_B","diff_B", binx, xmin, xmax, biny, ymin, ymax );
   diff_B->GetXaxis()->SetTitle("X (mm)");
   diff_B->GetYaxis()->SetTitle("Y (mm)");
   diff_B->GetZaxis()->SetTitle("Z (#mum)");

   TH1F *z_distrib = new TH1F("z_distrib","z_distr", 100, 600, 900);
   z_distrib->GetXaxis()->SetTitle("Z (#mum)");
   z_distrib->SetFillColor(kAzure-5);

   Map_diff->Add(ave,map2,-1,+1);

   Map_diff_B = (TH2F*)Map_diff->Clone("Map_diff11.32");
   diff_B = (TH2F*)Map_diff->Clone("diff_B");
   diff_B->Divide(diff_B, Map_diff,1,1);
   diff_B->Scale(11.29); 
   Map_diff_B->Add(Map_diff,diff_B,+1,-1);
   Map_diff_B->Scale(1000);


   float z;

   for (int i=1; i<=biny; i++){
      for (int j=1; j<=binx; j++){
         z = Map_diff->GetBinContent(j,i);
         if(z!=0) { z_distrib->Fill(z); }
      }
   }


   ostringstream rootfile;
   rootfile  << path << "diff_pannello_piastra_" << scan << ".root";
   TFile myroot(rootfile.str().c_str(), "RECREATE");
   cout << "Root output file: " << endl << rootfile.str() << endl;
  
   ostringstream Smap;
   Smap  << path  << "/diff_pan_piastra_"<< scan <<".png";  
   ostringstream Smapdiff;
   Smapdiff  << path  << "/diff_pan_piastraB_"<< scan <<".png";  
   ostringstream Sdistrib;
   Sdistrib  << path  << "/diff_pan_piastra_z_"<< scan <<".png"; 


   TCanvas *c0 = new TCanvas("c0", "c0", 200, 10, 1000,700);
   TPad *pad1 = new TPad("pad1","The pad with the histogram",0.03,0.02,0.97,0.9,0);
   pad1->Draw();
   TPaveLabel *title = new TPaveLabel(0.12,0.92,0.32,0.97,"Planarity Map");
   title->SetFillColor(kBlue-10);
   title->SetTextFont(42);
   title->Draw();
   pad1->cd();
   Map_diff->GetZaxis()->SetRange(540,640);
   Map_diff->SetMinimum(540);
   Map_diff->Draw("COLZ1TEXT45");
   Map_diff->SetStats(kFALSE); 
  TLine *l1 = new TLine(440,-40,440,1320);
  TLine *l2 = new TLine(840,-40,840,1320);
  TLine *l3 = new TLine(1320,-40,1320,1320);
  TLine *l4 = new TLine(1720,-40,1720,1320);
  l1->Draw(); l2->Draw();l3->Draw();l4->Draw();
   Map_diff->Write();
   c0->Print(Smap.str().c_str());



   TCanvas *c00 = new TCanvas("c00", "c00", 1000,700); 
   TPad *pad2 = new TPad("pad2","The pad with the histogram",0.03,0.02,0.97,0.9,0);
   pad2->Draw();
   TPaveLabel *title2 = new TPaveLabel(0.12,0.91,0.32,0.96,"Planarity Map");
   title2->SetFillColor(kBlue-10);
   title2->SetTextFont(42);
   title2->Draw();
   pad2->cd();
   Map_diff_B->Draw("COLZ1TEXT45");
   Map_diff_B->SetStats(kFALSE); 
  // myText(0.75,0.75,1,"Vacuum ON"); 
  l1->Draw(); l2->Draw();l3->Draw();l4->Draw();
   Map_diff_B->Write();
   c00->Print(Smapdiff.str().c_str());



   TCanvas *c1 = new TCanvas("c1", "c1", 1000,700); 
   TPad *pad3 = new TPad("pad3","The pad with the histogram",0.03,0.02,0.97,0.9,0);
   pad3->Draw();
   TPaveLabel *title3 = new TPaveLabel(0.12,0.92,0.35,0.97,"#Delta Z distribution");
   title3->SetFillColor(kBlue-10);
   title3->SetTextFont(42);
   title3->Draw();
   pad3->cd();
   z_distrib->Draw(); 
   TPaveLabel *label1 = new TPaveLabel(-3.5,700,-1,800,"Vacuum ON");
   label1->SetFillColor(42);
   label1->Draw();
  // myText(0.7,0.7,1,"Vacuum ON"); 
   z_distrib->Write();
   c1->Print(Sdistrib.str().c_str());
  
   myroot.Close(); 


  
   return; 
}
