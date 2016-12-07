#include <stdio.h>
#include <string>
#include <vector>


#define path "/home/atlas/Micromegas/M0Data/root_plot/"

void compare_reference (std::string scan1,std::string scan2, std::string title ){

  // style option

   gROOT->LoadMacro("AtlasUtils.C");   // myText
   gROOT->LoadMacro("AtlasStyle.C");   // ATLAS Style
   //SetAtlasStyle(); 
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".2f"); 
   gStyle->SetOptTitle(0);
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


// apro i file
   ostringstream infile1;
   infile1 << path << scan1;
 
   ostringstream infile2;
   infile2 << path  << scan2 ;
    
//   scan.erase(scan.end()-5,scan.end());

   TFile *s1 = new TFile(infile1.str().c_str());
   TH2F *map1 = (TH2F*)s1->Get("mean_map"); 
   TH2F *ave = (TH2F*)map1->Clone("averagee");
   ave->SetTitle("Average Map");
   //ave->Scale(0.001);

   TFile *s2 = new TFile(infile2.str().c_str());
   TH2F *map2 = (TH2F*)s2->Get("mean_map");
   TH2F *ave2 = (TH2F*)map2->Clone("averagee2");


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
   

   TH1F *z_distrib = new TH1F("z_distrib","z_distr", 100, -50, 50);
   z_distrib->GetXaxis()->SetTitle("Z (#mum)");
   z_distrib->SetFillColor(3);

   Map_diff->Add(ave,ave2,-1,+1);


   float z;

   for (int i=1; i<=biny; i++){
      for (int j=1; j<=binx; j++){
         z = Map_diff->GetBinContent(j,i);
          if(z!=0){ z_distrib->Fill(z); }
      }
   }


   ostringstream rootfile;
   rootfile  << path << title <<".root";
   TFile myroot(rootfile.str().c_str(), "RECREATE");
   cout << "Root output file: " << endl << rootfile.str() << endl;

   ostringstream Smap;
   Smap << title <<"_2D.png";
   ostringstream Sdistrib;
   Sdistrib << title <<"_distr.png";


   TCanvas *cc0 = new TCanvas("cc0", "cc0", 1000,700); 
   ave->Draw("colz,text45");
   ave->SetStats(kFALSE); 

   TCanvas *cc1 = new TCanvas("cc1", "cc1", 1000,700); 
   ave2->Draw("colz,text45");
   ave2->SetStats(kFALSE);

   TCanvas *c0 = new TCanvas("c0", "c0", 1000,700); 
   Map_diff->Draw("colz,text45");
   Map_diff->SetStats(kFALSE); 
   Map_diff->Write();
   c0->Print(Smap.str().c_str());

   TCanvas *c1 = new TCanvas(); 
   z_distrib->Draw(); 
   z_distrib->Write();
   c1->Print(Sdistrib.str().c_str());
  
   myroot.Close(); 


  
   return; 
}
