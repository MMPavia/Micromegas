#include <stdio.h>
#include <string>
#include <vector>
//#include "AtlasUtils.C"

//#define inpath "/home/atlas/Micromegas/M0Data/root_plot/"
#define outpath "/home/atlas/Micromegas/M0Data/root_plot/dummy5/"


void diff_vacuumoff_vacuumon (string scanoff, string scanon, string side){

  // style option
   gROOT->LoadMacro("AtlasUtils.C");   // myText
   gStyle->SetPalette(1);
   gStyle->SetOptStat(1101);
   gStyle->SetPaintTextFormat(".0f"); 
   gStyle->SetOptTitle(0);
   gStyle->SetPadRightMargin(0.15); 
   gStyle->SetNumberContours(100);


// apro i file
   ostringstream infileoff;
   infileoff << outpath  << scanoff ;
   scanoff.erase(scanoff.end()-5,scanoff.end());

   ostringstream infileon;
   infileon << outpath  << scanon ;
   scanon.erase(scanon.end()-5,scanon.end());

    cout << "file vacuum on : " <<infileon.str().c_str() << endl;
    cout << "file vacuum off : " <<infileoff.str().c_str() << endl;


   TFile *son = new TFile(infileon.str().c_str());
   TH2F *mapon = (TH2F*)son->Get("map"); 

   TFile *soff = new TFile(infileoff.str().c_str());
   TH2F *mapoff = (TH2F*)soff->Get("map"); 

   Int_t binx = mapon->GetNbinsX();
   Int_t biny = mapon->GetNbinsY();
   cout << binx << " " << biny << endl;

   float xmin = -40;
   float xmax = 2280;
   float ymin = -40;
   float ymax =1320;
   float xstep = (xmax-xmin)/binx;
   float ystep = (ymax-ymin)/biny;



   TH1F *z_distrib = new TH1F("Difference Distribution","z_distrib", 60, -20 , 100);
   z_distrib->GetXaxis()->SetTitle("Z (mm)");
   z_distrib->SetFillColor(kAzure-5);


   TH2F *Map_diff= new TH2F("Map_diff","Map_diff", binx, xmin, xmax, biny, ymin, ymax );
   Map_diff->GetXaxis()->SetTitle("X (mm)");
   Map_diff->GetYaxis()->SetTitle("Y (mm)");
   Map_diff->GetZaxis()->SetTitle("Z (#mum)");
   Map_diff->Add(mapoff,mapon,+1,-1);
   Map_diff->Scale(1000);

   float z, max_z, min_z;
   max_z = -100000;
   min_z = 100000;


   for (int i=1; i<=biny; i++){
      for (int j=1; j<=binx; j++){
         z = Map_diff->GetBinContent(j,i);
         if(z!=0) { z_distrib->Fill(z); }
          if(z!=0 && z<min_z){min_z = z;}
          if(z!=0 && z>max_z){max_z = z;}

      }
   }

   float meanZ = z_distrib->GetMean();
   cout << "*******************" << endl;
   cout << "max_z: "  << max_z << endl;
   cout << "min_z: "  << min_z << endl;
   cout << "mean: "  << meanZ << endl;
   cout << "*******************" << endl;


   ostringstream Dmap;
   //Dmap  << outpath  << "mapdiff_Voff-Von_side_" << side <<".png";  
   Dmap  << outpath  << "mapdiff_" << side << "-400mb.png";  
   ostringstream Ddistr;
   //Ddistr  << outpath  << "distridiff_Voff-Von_side_" << side <<".png";  
   Ddistr  << outpath  << "distridiff_"<< side <<"-400mb.png";  


   TCanvas *c00 = new TCanvas("c00", "c00", 1000,700); 
   TPad *pad2 = new TPad("pad2","The pad with the histogram",0.03,0.02,0.97,0.9,0);
   pad2->Draw();
   TString title = "Difference " + side + " -400mb " ;
   TPaveLabel *title2 = new TPaveLabel(0.12,0.91,0.32,0.96,title);
   title2->SetFillColor(kBlue-10);
   title2->SetTextFont(42);
   title2->Draw();
   pad2->cd();
   //Map_diff->GetZaxis()->SetRangeUser(0,100.);
   Map_diff->Draw("COLZ1TEXT45");
   Map_diff->SetStats(kFALSE); 
   TLine *l1 = new TLine(440,-40,440,1320);
   TLine *l2 = new TLine(840,-40,840,1320);
   TLine *l3 = new TLine(1320,-40,1320,1320);
   TLine *l4 = new TLine(1720,-40,1720,1320);
   l1->Draw(); l2->Draw();l3->Draw();l4->Draw();
   c00->Print(Dmap.str().c_str());



   TCanvas *c1 = new TCanvas("c1", "c1", 1000,700);
   TPad *pad3 = new TPad("pad3","The pad with the histogram",0.03,0.02,0.97,0.9,0);
   pad3->Draw();
   TPaveLabel *title3 = new TPaveLabel(0.12,0.92,0.35,0.97,"Difference  Distribution");
   title3->SetFillColor(kBlue-10);
   title3->SetTextFont(42);
   title3->Draw();
   pad3->cd();
   z_distrib->Draw();
   TPaveLabel *label1 = new TPaveLabel(-3.5,700,-1,800,"Vacuum ON");
   label1->SetFillColor(42);
   label1->Draw();
   z_distrib->Write();
   c1->Print(Ddistr.str().c_str());




/*
  TCanvas *c1 = new TCanvas("c1", "c1", 1000,700);
  mapon->Draw("COLZ1TEXT45"); 
  l1->Draw(); l2->Draw();l3->Draw();l4->Draw();
  mapon->Draw("COLZ1TEXT45"); 
  
  TCanvas *c2 = new TCanvas("c2", "c2", 1000,700);
  mapoff->Draw("COLZ1TEXT45"); 
  l1->Draw(); l2->Draw();l3->Draw();l4->Draw();
  mapoff->Draw("COLZ1TEXT45"); 
*/

   return;
}
