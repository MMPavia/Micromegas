#include <stdio.h>
#include <string>
#include <vector>
//#include "AtlasUtils.C"

#define inpath "/home/atlas/Micromegas/M05Data/root_plot/"
#define outpath "/home/atlas/Micromegas/M05Data/root_plot/modulo05/"


void diff_pannello_piastre (string scan){

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
   // infile1 << inpath << "ReferenceTablePlate.root"; 
   //infile1 << inpath << "../reference/mean_and_rep_gauge.root";
   infile1 << inpath << "../reference/mean_and_rep_plates_gauge_noopt.root";

   //infile1 << inpath << "../reference/mean_and_rep_plates_laser.root";  // Bob & Mari 24.02.17 *** nota bene: reference file del 13 febbraio

   ostringstream infile2;
   infile2 << outpath  << scan ;

   scan.erase(scan.end()-5,scan.end());

   TFile *s1 = new TFile(infile1.str().c_str());
   TH2F *map1 = (TH2F*)s1->Get("mean_map"); 
   TH2F *ave = (TH2F*)map1->Clone("average");
   ave->SetTitle("Average Map");
   ave->Scale(0.001);

   TFile *s2 = new TFile(infile2.str().c_str());
   TH2F *map2 = (TH2F*)s2->Get("map"); 
   TH1F *dist = (TH1F*)s2->Get("z_corr");

   Int_t binx = ave->GetNbinsX();
   Int_t biny = ave->GetNbinsY();
   cout << binx << " " << biny << endl;

   float xmin = 253/10;
   float xmax = 2414/10;
   float ymin = 162;
   float ymax =1239;
   float xstep = (xmax-xmin)/binx-1;
   float ystep = (ymax-ymin)/biny-1;


   TH2F *Map_diff= new TH2F("Map_diff","Map_diff", binx, xmin, xmax, biny, ymin, ymax );
   Map_diff->GetXaxis()->SetTitle("X (mm)");
   Map_diff->GetYaxis()->SetTitle("Y (mm)");
   Map_diff->GetZaxis()->SetTitle("Z (#mum)");

   TH2F *diff_ave= new TH2F("diff wrt mean","diff wrt mean", binx, xmin, xmax, biny, ymin, ymax );
   diff_ave->GetXaxis()->SetTitle("X (mm)");
   diff_ave->GetYaxis()->SetTitle("Y (mm)");
   diff_ave->GetZaxis()->SetTitle("Z (mm)");

   TH1F *z_distrib = new TH1F("Z Distribution","z_distrib", 100, 11.40, 12.0);
   z_distrib->GetXaxis()->SetTitle("Z (mm)");
   z_distrib->SetFillColor(kAzure-5);

   Map_diff->Add(ave,map2,-1,+1);

   float zref,zmap, z, max_z, min_z;
   max_z = -100000;
   min_z = 100000;

   for (int i=1; i<=biny; i++){
      for (int j=1; j<=binx; j++){
         zref= ave->GetBinContent(j,i);
         zmap = map2->GetBinContent(j,i);
         z = Map_diff->GetBinContent(j,i);
         if(z!=0) {
	   z_distrib->Fill(z); 
           cout << zref << " " << zmap << " " << z << endl;
	}
         if(z!=0 && z<min_z){min_z = z;}
         if(z!=0 && z>max_z){max_z = z;}
      }
   }

   diff_ave = (TH2F*)Map_diff->Clone("diff_ave");
   diff_ave->Divide(Map_diff); 
   Float_t averageZ  = z_distrib->GetMean();
   cout <<  "average : " << averageZ << endl; 
   diff_ave->Scale(averageZ);
   diff_ave->Add(Map_diff,diff_ave,+1,-1);
   diff_ave->Scale(1000);

   cout << "*******************" << endl;
   cout << "max_z: "  << max_z << endl;
   cout << "min_z: "  << min_z << endl;
   cout << "mean: "  << averageZ << endl;
   cout << "*******************" << endl;




   ostringstream rootfile;
   rootfile  << outpath << "diff_pannello_piastra_" << scan << ".root";
   TFile myroot(rootfile.str().c_str(), "RECREATE");
   cout << "Root output file: " << endl << rootfile.str() << endl;

   ostringstream Smap;
   Smap  << outpath  << "/diff_pan_piastra_"<< scan <<".png";  
   ostringstream Smapdiff;
   Smapdiff  << outpath  << "/diff_pan_piastra_"<< scan <<".png";  
   ostringstream Sdistrib;
   Sdistrib  << outpath  << "/diff_pan_piastra_z_"<< scan <<".png"; 



   TCanvas *c = new TCanvas("ref","ref",200, 10, 1000,700); 
   map1->Draw("COLZ1TEXT45");
   map1->SetStats(kFALSE);
   
   TCanvas *cc = new TCanvas("map","map ",200, 10, 1000,700); 
   map2->Draw("COLZ1TEXT45");
   map2->SetStats(kFALSE);
   
   TCanvas *ccc = new TCanvas(); 
   dist->Draw();


   TCanvas *c0 = new TCanvas("map corrected", "map corrected", 200, 10, 1000,700);
   Map_diff->Draw("COLZ1TEXT45");
   Map_diff->SetStats(kFALSE); 
   Map_diff->Write();
   c0->Print(Smap.str().c_str());


   TCanvas *c00 = new TCanvas("map corr wrt mean", "map corr wrt mean", 1000,700); 
   diff_ave->Draw("COLZ1TEXT45");
   diff_ave->SetStats(kFALSE); 
//   TLine *l1 = new TLine(440,-40,440,1320);
//   TLine *l2 = new TLine(840,-40,840,1320);
//   TLine *l3 = new TLine(1320,-40,1320,1320);
//   TLine *l4 = new TLine(1720,-40,1720,1320);
//   l1->Draw(); l2->Draw();l3->Draw();l4->Draw();
   diff_ave->Write();
   c00->Print(Smapdiff.str().c_str());

   TCanvas *c1 = new TCanvas("c1", "c1", 1000,700); 
   z_distrib->Draw(); 
   z_distrib->Write();
   c1->Print(Sdistrib.str().c_str());

   myroot.Close(); 


   return;
}
