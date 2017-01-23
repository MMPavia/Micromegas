#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
//#include "AtlasStyle.C"

#define inpath "/home/atlas/Micromegas/M05Data/mapping_stiffback/"
#define outpath "/home/atlas/Micromegas/M05Data/root_plot/stiffback/"

void repeatibility_and_mean_stiffback (string scan1, string scan2, string scan3){


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
   ostringstream file1;
   file1 << inpath  << scan1;
   scan1.erase(scan1.end()-4,scan1.end());
   scan1.erase(scan1.begin(),scan1.begin()+8);

   ostringstream file2;
   file2 << inpath  << scan2;
   scan2.erase(scan2.end()-4,scan2.end());
   scan2.erase(scan2.begin(),scan2.begin()+8);

   ostringstream file3;
   file3 << inpath  << scan3;
   scan3.erase(scan3.end()-4,scan3.end());
   scan3.erase(scan3.begin(),scan3.begin()+8);

   int nxbin=5;
   int nybin=4;
   float xmin = -40;
   float xmax = 2280;
   float ymin = -40;
   float ymax =1320;

   TH2F *map_s1 = new TH2F("map_s1","map_s1", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s1->GetXaxis()->SetTitle("X (mm)");
   map_s1->GetYaxis()->SetTitle("Y (mm)");
   map_s1->GetZaxis()->SetTitle("Z (#mum)");

   TH2F *map_s2 = new TH2F("map_s2","map_s2", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s2->GetXaxis()->SetTitle("X (mm)");
   map_s2->GetYaxis()->SetTitle("Y (mm)");
   map_s2->GetZaxis()->SetTitle("Z (#mum)");

   TH2F *map_s3 = new TH2F("map_s3","map_s3", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s3->GetXaxis()->SetTitle("X (mm)");
   map_s3->GetYaxis()->SetTitle("Y (mm)");
   map_s3->GetZaxis()->SetTitle("Z (#mum)");
 
   TH1F *distz_s1 = new TH1F("distz_s1","distz_s1", 30, 100, 600); 
   distz_s1->GetXaxis()->SetTitle("#mum");
   distz_s1->GetYaxis()->SetTitle("counts");

   TH1F *distz_s2 = new TH1F("distz_s2","distz_s2", 30, 100, 600); 
   distz_s2->GetXaxis()->SetTitle("#mum");
   distz_s2->GetYaxis()->SetTitle("counts");

   TH1F *distz_s3 = new TH1F("distz_s3","distz_s3",  30, 100, 600); 
   distz_s3->GetXaxis()->SetTitle("#mum");
   distz_s3->GetYaxis()->SetTitle("counts");

   TH2F *mean_map = new TH2F("mean_map", "Average Map", nxbin, xmin, xmax, nybin, ymin, ymax );
   mean_map->GetXaxis()->SetTitle("X (mm)");
   mean_map->GetYaxis()->SetTitle("Y (mm)");
   mean_map->GetZaxis()->SetTitle("Z (#mum)");

   TH1F *mean_distrib = new TH1F("Average_distrib","Average Distribution",  30, 0, 600);
   mean_distrib->GetXaxis()->SetTitle("Z (#mum)");
   mean_distrib->SetFillColor(3);

   TH2F *hzmin= new TH2F("hzmin","hzmin", nxbin, xmin, xmax, nybin, ymin, ymax );
   hzmin->GetXaxis()->SetTitle("X (mm)");
   hzmin->GetYaxis()->SetTitle("Y (mm)");
   hzmin->GetZaxis()->SetTitle("Z (#mum)");
   
   TH2F *hzmax= new TH2F("hzmax","hzmax", nxbin, xmin, xmax, nybin, ymin, ymax );
   hzmax->GetXaxis()->SetTitle("X (mm)");
   hzmax->GetYaxis()->SetTitle("Y (mm)");
   hzmax->GetZaxis()->SetTitle("Z (#mum)");
   
   TH2F *hdz= new TH2F("Repeatability Map","Repeatability Map", nxbin, xmin, xmax, nybin, ymin, ymax );
   hdz->GetXaxis()->SetTitle("X (mm)");
   hdz->GetYaxis()->SetTitle("Y (mm)");
   hdz->GetZaxis()->SetTitle("Z (#mum)");
   
   TH1F *dz_distrib = new TH1F("dz_distrib","Repeatability Distribution", 20, -5, 20);
   dz_distrib->GetXaxis()->SetTitle("Z (#mum)");
   dz_distrib->SetFillColor(2);

   TH2F *std_map= new TH2F("std_map","Std Map", nxbin, xmin, xmax, nybin, ymin, ymax );
   std_map->GetXaxis()->SetTitle("X (mm)");
   std_map->GetYaxis()->SetTitle("Y (mm)");
   std_map->GetZaxis()->SetTitle("Z (#mum)");

   TH1F *std_distrib = new TH1F("std_distrib","Std Distribution", 20, -5, 15);
   std_distrib->GetXaxis()->SetTitle("Z (#mum)");
   std_distrib->SetFillColor(4);


   Double_t dd1, dd2, dd3, dd4, dd5, m1, m2, mean; 
   Double_t d1, d2, d3, d4, d5, dd, mean2, max, min, dz; 
   Double_t dz1_mean, dz2_mean, dz3_mean, dz4_mean, dz5_mean, std;

   Double_t scan[7][20]={0};
   Double_t x,y, opt, laser, tesa, temp1, temp2, coord;

// leggo file e riempio array con coord

    ifstream in1(file1.str().c_str());
    for (int j=0; j<=19; j++){
           in1 >> x >> y >> opt >> laser >> tesa >> temp1 >> temp2;
           coord = (tesa - opt)-113;
           scan[0][j]=coord*1000;
    }


    ifstream in2(file2.str().c_str());
    for (int j=0; j<=19; j++){
           in2 >> x >> y >> opt >> laser >> tesa >> temp1 >> temp2;
           coord = (tesa - opt)-113;
           scan[1][j]=coord*1000;
    }


    ifstream in3(file3.str().c_str());
    for (int j=0; j<=19; j++){
           in3 >> x >> y >> opt >> laser >> tesa >> temp1 >> temp2;
           coord = (tesa - opt)-113;
           scan[2][j]=coord*1000;
    }



// leggo file e riempio array con coord x e y
  ifstream in4 (file1.str().c_str());
  for (int j=0; j<=19; j++){
             in4 >> x >> y >> opt >> laser >> tesa >> temp1 >> temp2;
	     scan[5][j]=x;
	     scan[6][j]=y;
   }



// leggo array e calcolo media e std di ogni punto
   for (int j=0; j<=19; j++){
      d1 = scan[0][j];
      d2 = scan[1][j];
      d3 = scan[2][j];
      mean2 = (d1+d2+d3)/3;
      map_s1->Fill(scan[5][j],scan[6][j], d1);
      map_s2->Fill(scan[5][j],scan[6][j], d2);
      map_s3->Fill(scan[5][j],scan[6][j], d3);
      distz_s1->Fill(d1);
      distz_s2->Fill(d2);
      distz_s3->Fill(d3);
      mean_map->Fill(scan[5][j],scan[6][j], mean2);
      mean_distrib->Fill(mean2); 
      dz1_mean = scan[0][j] - mean2;
      dz2_mean = scan[1][j] - mean2;
      dz3_mean = scan[2][j] - mean2;
      std = sqrt((dz1_mean*dz1_mean + dz2_mean*dz2_mean + dz3_mean*dz3_mean)/2);
      std_distrib->Fill(std);
      std_map->Fill(scan[5][j],scan[6][j],std);
   }


// leggo array e calcolo rep come dz

   for (int j=0; j<=19; j++){	
      max=-1000; 
      min=1000;
      for (int i=0; i<=2; i++){
          if(scan[i][j]<min){min = scan[i][j];}
          if(scan[i][j]>max){max = scan[i][j];}
      }
    dz=max-min;
    dz_distrib->Fill(dz);
    hdz->Fill(scan[5][j],scan[6][j],dz);
   }


 // create a root file for the histograms; 
  ostringstream rootfile;
  rootfile  << outpath <<"mean_and_rep_Stiffback_" << scan1 << "_" << scan2 << "_" << scan3  <<".root";
  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;

  ostringstream Mean_map;
  Mean_map << outpath << "/mean_map_"<< scan1 << "_" << scan2 << "_" << scan3  <<".png";

  ostringstream Mean_distrib;
  Mean_distrib << outpath << "/mean_distrib_"<< scan1 << "_" << scan2 << "_" << scan3  <<".png";

  ostringstream Dz_map;
  Dz_map << outpath << "/dz_map_"<< scan1 << "_" << scan2 << "_" << scan3  <<".png";

  ostringstream Dz_distrib;
  Dz_distrib << outpath << "/dz_distrib_"<< scan1 << "_" << scan2 << "_" << scan3  <<".png";

  ostringstream Std_map;
  Std_map << outpath << "/std_map_"<< scan1 << "_" << scan2 << "_" << scan3  <<".png";

  ostringstream Std_distrib;
  Std_distrib << outpath << "/std_distrib_"<< scan1 << "_" << scan2 << "_" << scan3  <<".png";


/*   TCanvas *c0 = new TCanvas("c0","c0",1000,700);
   map_s1->SetMarkerSize(0.8);
   map_s1->Draw("colz,text45");
   map_s1->SetStats(kFALSE);
   map_s1->Write();

   TCanvas *c1 = new TCanvas("c1","c1",1000,700);
   map_s2->SetMarkerSize(0.8);
   map_s2->Draw("colz,text45");
   map_s2->SetStats(kFALSE);
   map_s2->Write();

   TCanvas *c2 = new TCanvas("c2","c2",1000,700);
   map_s3->SetMarkerSize(0.8);
   map_s3->Draw("colz,text45");
   map_s3->SetStats(kFALSE);
   map_s3->Write();

   TCanvas *c21 = new TCanvas("c21","c21",1000,700);
   distz_s1->Draw();
   distz_s1->Write();

   TCanvas *c22 = new TCanvas("c22","c22",1000,700);
   distz_s2->Draw();
   distz_s2->Write();

   TCanvas *c23 = new TCanvas("c23","c23",1000,700);
   distz_s3->Draw();
   distz_s3->Write();
*/

   TCanvas *cc2 = new TCanvas("cc2","cc2",1000,700);  
   mean_map->SetMarkerSize(0.8);
   mean_map->SetStats(kFALSE);
   mean_map->Draw("colz,text45"); 
   mean_map->Write();
   cc2->Print(Mean_map.str().c_str());

   TCanvas *cc3 = new TCanvas("cc3","cc3",1000,700); 
   mean_distrib->Draw(); 
   mean_distrib->Write();
   cc3->Print(Mean_distrib.str().c_str());

   TCanvas *c9 = new TCanvas("c9","c9",1000,700); 
   hdz->SetMarkerSize(0.8); 
   hdz->SetStats(kFALSE);
   hdz->Draw("colz,text0"); 
   hdz->Write();
   c9->Print(Dz_map.str().c_str());

   TCanvas *c6 = new TCanvas("c6","c6",1000,700); 
   dz_distrib->Draw(); 
   dz_distrib->Write();
   c6->Print(Dz_distrib.str().c_str());

   TCanvas *c16 = new TCanvas("c16","c16",1000,700);  
   std_map->SetMarkerSize(0.8);
   std_map->SetStats(kFALSE);
   std_map->Draw("colz,text0"); 
   std_map->Write();
   c16->Print(Std_map.str().c_str());

   TCanvas *c26 = new TCanvas("c26","c26",1000,700);  
   std_distrib->Draw(); 
   std_distrib->Write();
   c26->Print(Std_distrib.str().c_str());


  myroot.Close();

   return; 
}
