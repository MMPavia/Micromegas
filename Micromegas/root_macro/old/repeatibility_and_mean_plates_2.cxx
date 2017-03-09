#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
//#include "AtlasStyle.C"

#define inpath "/home/atlas/Micromegas/M05Data/mapping/" 
#define outpath "/home/atlas/Micromegas/M05Data/root_plot/"

//void repeatibility_and_mean_plates (string scan1, string scan2, string scan3, string scan4, string scan5){
void repeatibility_and_mean_plates_2 (bool plates, string scan1, string scan2){

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

   ostringstream file2;
   file2 << inpath  << scan2;
   scan2.erase(scan2.end()-4,scan2.end());
   scan2.erase(scan2.begin(),scan2.begin()+13);

   /*
   ostringstream file5;
   file5 << inpath  << scan5;
   scan5.erase(scan5.end()-4,scan5.end());
   scan5.erase(scan5.begin(),scan5.begin()+13);
*/

   int nxbin=5;
   int nybin=14;
   float xmin = 258/10;
   float xmax = 656/10;
   float ymax = 1258;
   float ymin = 130;
  

   TH2F *map_laser_s1 = new TH2F("map_laser_s1","map_laser_s1", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_laser_s1->GetXaxis()->SetTitle("X (cm)");
   map_laser_s1->GetYaxis()->SetTitle("Y (mm)");
   map_laser_s1->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *map_laser_s2 = new TH2F("map_laser_s2","map_laser_s2", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_laser_s2->GetXaxis()->SetTitle("X (cm)");
   map_laser_s2->GetYaxis()->SetTitle("Y (mm)");
   map_laser_s2->GetZaxis()->SetTitle("Z (#mu m)");
/*
   TH2F *map_laser_s3 = new TH2F("map_laser_s3","map_laser_s3", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_laser_s3->GetXaxis()->SetTitle("X (cm)");
   map_laser_s3->GetYaxis()->SetTitle("Y (mm)");
   map_laser_s3->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *map_laser_s4 = new TH2F("map_laser_s4","map_laser_s4", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_laser_s4->GetXaxis()->SetTitle("X (cm)");
   map_laser_s4->GetYaxis()->SetTitle("Y (mm)");
   map_laser_s4->GetZaxis()->SetTitle("Z (#mu m)");


   TH2F *map_laser_s5 = new TH2F("map_laser_s5","map_laser_s5", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_laser_s5->GetXaxis()->SetTitle("X (cm)");
   map_laser_s5->GetYaxis()->SetTitle("Y (mm)");
   map_laser_s5->GetZaxis()->SetTitle("Z (#mu m)");
*/

   TH2F *map_gauge_s1 = new TH2F("map_gauge_s1","map_gauge_s1", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_gauge_s1->GetXaxis()->SetTitle("X (cm)");
   map_gauge_s1->GetYaxis()->SetTitle("Y (mm)");
   map_gauge_s1->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *map_gauge_s2 = new TH2F("map_gauge_s2","map_gauge_s2", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_gauge_s2->GetXaxis()->SetTitle("X (cm)");
   map_gauge_s2->GetYaxis()->SetTitle("Y (mm)");
   map_gauge_s2->GetZaxis()->SetTitle("Z (#mu m)");
/*
   TH2F *map_gauge_s3 = new TH2F("map_gauge_s3","map_gauge_s3", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_gauge_s3->GetXaxis()->SetTitle("X (cm)");
   map_gauge_s3->GetYaxis()->SetTitle("Y (mm)");
   map_gauge_s3->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *map_gauge_s4 = new TH2F("map_gauge_s4","map_gauge_s4", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_gauge_s4->GetXaxis()->SetTitle("X (cm)");
   map_gauge_s4->GetYaxis()->SetTitle("Y (mm)");
   map_gauge_s4->GetZaxis()->SetTitle("Z (#mu m)");


   TH2F *map_gauge_s5 = new TH2F("map_gauge_s5","map_gauge_s5", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_gauge_s5->GetXaxis()->SetTitle("X (cm)");
   map_gauge_s5->GetYaxis()->SetTitle("Y (mm)");
   map_gauge_s5->GetZaxis()->SetTitle("Z (#mu m)");
*/


   int ndistbin = 40; 
   float xdistmin = -50; 
   float xdistmax = 80; 
   
   if(!plates){
      ndistbin = 50; 
      xdistmin = -500; 
      xdistmax = 500; 
   }


   TH1F *distz_laser_s1 = new TH1F("distz_laser_s1","distz_laser_s1", ndistbin,xdistmin, xdistmax  ); 
   distz_laser_s1->GetXaxis()->SetTitle("#mu ");
   distz_laser_s1->GetYaxis()->SetTitle("counts");
   distz_laser_s1->SetFillColor(2);

   TH1F *distz_laser_s2 = new TH1F("distz_laser_s2","distz_laser_s2",  ndistbin,xdistmin, xdistmax  ); 
   distz_laser_s2->GetXaxis()->SetTitle("#mu ");
   distz_laser_s2->GetYaxis()->SetTitle("counts");
   distz_laser_s2->SetFillColor(2);
/*
   TH1F *distz_laser_s3 = new TH1F("distz_laser_s3","distz_laser_s3",  ndistbin,xdistmin, xdistmax  ); 
   distz_laser_s3->GetXaxis()->SetTitle("#mu ");
   distz_laser_s3->GetYaxis()->SetTitle("counts");
   distz_laser_s3->SetFillColor(2);
      
   TH1F *distz_laser_s4 = new TH1F("distz_laser_s4","distz_laser_s4",  ndistbin,xdistmin, xdistmax  ); 
   distz_laser_s4->GetXaxis()->SetTitle("#mu ");
   distz_laser_s4->GetYaxis()->SetTitle("counts");
   distz_laser_s4->SetFillColor(2);


   TH1F *distz_laser_s5 = new TH1F("distz_laser_s5","distz_laser_s5",  ndistbin,xdistmin, xdistmax ); 
   distz_laser_s5->GetXaxis()->SetTitle("#mu ");
   distz_laser_s5->GetYaxis()->SetTitle("counts");
   distz_laser_s5->SetFillColor(2);
*/

   TH1F *distz_gauge_s1 = new TH1F("distz_gauge_s1","distz_gauge_s1", ndistbin,xdistmin, xdistmax  ); 
   distz_gauge_s1->GetXaxis()->SetTitle("#mu ");
   distz_gauge_s1->GetYaxis()->SetTitle("counts");
   distz_gauge_s1->SetFillColor(4);

   TH1F *distz_gauge_s2 = new TH1F("distz_gauge_s2","distz_gauge_s2",  ndistbin,xdistmin, xdistmax  ); 
   distz_gauge_s2->GetXaxis()->SetTitle("#mu ");
   distz_gauge_s2->GetYaxis()->SetTitle("counts");
   distz_gauge_s2->SetFillColor(4);
/*
   TH1F *distz_gauge_s3 = new TH1F("distz_gauge_s3","distz_gauge_s3",  ndistbin,xdistmin, xdistmax  ); 
   distz_gauge_s3->GetXaxis()->SetTitle("#mu ");
   distz_gauge_s3->GetYaxis()->SetTitle("counts");
   distz_gauge_s3->SetFillColor(4);
      
   TH1F *distz_gauge_s4 = new TH1F("distz_gauge_s4","distz_gauge_s4",  ndistbin,xdistmin, xdistmax  ); 
   distz_gauge_s4->GetXaxis()->SetTitle("#mu ");
   distz_gauge_s4->GetYaxis()->SetTitle("counts");
   distz_gauge_s4->SetFillColor(4);


   TH1F *distz_gauge_s5 = new TH1F("distz_gauge_s5","distz_gauge_s5",  ndistbin,xdistmin, xdistmax ); 
   distz_gauge_s5->GetXaxis()->SetTitle("#mu ");
   distz_gauge_s5->GetYaxis()->SetTitle("counts");
   distz_gauge_s5->SetFillColor(4);
*/


   TH2F *mean_laser_map = new TH2F("mean_laser_map", "Average Laser Map", nxbin, xmin, xmax, nybin, ymin, ymax );
   mean_laser_map->GetXaxis()->SetTitle("X (mm)");
   mean_laser_map->GetYaxis()->SetTitle("Y (mm)");
   mean_laser_map->GetZaxis()->SetTitle("Z (#mum)");

   TH1F *mean_laser_distrib = new TH1F("Average_laser_distrib","Average Laser Distribution", ndistbin,xdistmin, xdistmax );
   mean_laser_distrib->GetXaxis()->SetTitle("Z (#mum)");
   mean_laser_distrib->SetFillColor(3);

   TH2F *mean_gauge_map = new TH2F("mean_gauge_map", "Average Gauge Map", nxbin, xmin, xmax, nybin, ymin, ymax );
   mean_gauge_map->GetXaxis()->SetTitle("X (mm)");
   mean_gauge_map->GetYaxis()->SetTitle("Y (mm)");
   mean_gauge_map->GetZaxis()->SetTitle("Z (#mum)");

   TH1F *mean_gauge_distrib = new TH1F("Average_gauge_distrib","Average Gauge Distribution", ndistbin,xdistmin, xdistmax );
   mean_gauge_distrib->GetXaxis()->SetTitle("Z (#mum)");
   mean_gauge_distrib->SetFillColor(3);

   /*
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

   TH1F *dz_distrib = new TH1F("dz_distrib","Repeatability Distribution", 50, -5, 95);
   dz_distrib->GetXaxis()->SetTitle("Z (#mum)");
   dz_distrib->SetFillColor(2);
   */


   TH2F *std_map_laser= new TH2F("std_map_laser","std_map_laser", nxbin, xmin, xmax, nybin, ymin, ymax );
   std_map_laser->GetXaxis()->SetTitle("X (mm)");
   std_map_laser->GetYaxis()->SetTitle("Y (mm)");
   std_map_laser->GetZaxis()->SetTitle("Z (#mum)");

   TH1F *std_laser_distrib = new TH1F("std_distrib_laser","std laser Distribution", 30, -2, 28);
   std_laser_distrib->GetXaxis()->SetTitle("Z (#mum)");
   std_laser_distrib->SetFillColor(6);

   TH2F *std_map_gauge= new TH2F("std_map_gauge","std_map_gauge", nxbin, xmin, xmax, nybin, ymin, ymax );
   std_map_gauge->GetXaxis()->SetTitle("X (mm)");
   std_map_gauge->GetYaxis()->SetTitle("Y (mm)");
   std_map_gauge->GetZaxis()->SetTitle("Z (#mum)");

   TH1F *std_gauge_distrib = new TH1F("std_distrib_gauge","std gauge Distribution", 30, -2, 28);
   std_gauge_distrib->GetXaxis()->SetTitle("Z (#mum)");
   std_gauge_distrib->SetFillColor(6);


   //Float_t dd1, dd2, dd3, dd4, dd5, mean; 
   Float_t ld1, ld2, ld3, ld4, ld5, ldd, lmean2, lmax, lmin, ldz; 
   Float_t gd1, gd2, gd3, gd4, gd5, gdd, gmean2, gmax, gmin, gdz; 
   Float_t ldz1_mean, ldz2_mean, ldz3_mean, ldz4_mean, ldz5_mean, lstd;
   Float_t gdz1_mean, gdz2_mean, gdz3_mean, gdz4_mean, gdz5_mean, gstd;
   Int_t n = 64; 

   Float_t lscan[7][266]={0};
   Float_t gscan[7][266]={0};
   Float_t x,y, opt, laser, gauge, temp1, temp2, lcoord, gcoord ;


// leggo file e riempio array con coord

    ifstream in1(file1.str().c_str());
    for (int j=0; j<n; j++){
           in1 >> x >> y >> opt >> laser >> gauge >>   temp1 >> temp2;
           lcoord = (laser - opt);
           lscan[0][j]=lcoord*1000;
           gcoord = (gauge - opt);
           gscan[0][j]=gcoord*1000;
   	   cout << lscan[0][j] << " " << gscan[0][j] << endl;
    }


    ifstream in2(file2.str().c_str());
    for (int j=0; j<n; j++){
           in2 >> x >> y >> opt >> laser >> gauge >>  temp1 >> temp2;
           lcoord = (laser - opt);
           lscan[1][j]=lcoord*1000;
           gcoord = (gauge - opt);
           gscan[1][j]=gcoord*1000;
   	   cout << lscan[1][j] << " " << gscan[1][j] << endl;
    }
/*
    ifstream in3(file3.str().c_str());
    for (int j=0; j<n; j++){
           in3 >> x >> y >> opt >> laser  >> gauge >>  temp1 >> temp2;
           lcoord = (laser - opt);
           lscan[2][j]=lcoord*1000;
           gcoord = (gauge - opt);
           gscan[2][j]=gcoord*1000;
   	   cout << lscan[2][j] << " " << gscan[2][j] << endl;
    }

    ifstream in4(file4.str().c_str());
    for (int j=0; j<n; j++){
           in4 >> x >> y >> opt >> laser >> gauge >>  temp1 >> temp2;
           lcoord = (laser - opt);
           lscan[3][j]=lcoord*1000;
           gcoord = (gauge - opt);
           gscan[3][j]=gcoord*1000;
   	   cout << lscan[3][j] << " " << gscan[3][j] << endl;
    }



    ifstream in5(file5.str().c_str());
    for (int j=0; j<n; j++){
           in5 >> x >> y >> opt >> laser >> gauge >>  temp1 >> temp2;
           lcoord = (laser - opt);
           lscan[4][j]=lcoord*1000;
           gcoord = (gauge - opt);
           gscan[4][j]=gcoord*1000;
   	   cout << lscan[4][j] << " " << gscan[4][j] << endl;
    }
    
*/
/*
// tolgo i buchi
   for (int i=0; i<=4; i++){
      for (int j=0; j<=n; j++){
            if(lscan[i][j]<-200) {lscan[i][j]=(lscan[i][j-1]+lscan[i][j+1])/2;}
      }
   }

*/
// leggo file e riempio array con lcoord x e y
  ifstream in6 (file1.str().c_str());
  for (int j=0; j<=n; j++){
             in6 >> x >> y >> opt >> laser >> gauge >> temp1 >> temp2;
             lscan[5][j]=x/10;
             lscan[6][j]=-y;
//	     cout << lscan[5][j] << " " << lscan[6][j] << endl;
   }


// leggo array e calcolo media e std di ogni punto
   for (int j=0; j<=n; j++){
      ld1 = lscan[0][j];
      ld2 = lscan[1][j];
//      ld3 = lscan[2][j];
//      ld4 = lscan[3][j];
//    ld5 = lscan[4][j];
      cout <<"laser " <<  ld1 << " " << ld2 << " " << ld3 << " " << ld4 << endl; 
      gd1 = gscan[0][j];
      gd2 = gscan[1][j];
//      gd3 = gscan[2][j];
//      gd4 = gscan[3][j];
//    gd5 = gscan[4][j];
      cout << "gauge " << gd1 << " " << gd2 << " " << gd3 << " " << gd4 << endl; 
    
//    mean2 = (d1+d2+d3+d4+d5)/5;
      lmean2 = (ld1+ld2)/2;
      gmean2 = (gd1+gd2)/2;

      map_laser_s1->Fill(lscan[5][j],lscan[6][j], ld1);
      map_laser_s2->Fill(lscan[5][j],lscan[6][j], ld2);
//      map_laser_s3->Fill(lscan[5][j],lscan[6][j], ld3);
//      map_laser_s4->Fill(lscan[5][j],lscan[6][j], ld4);
//    map_laser_s5->Fill(lscan[5][j],lscan[6][j], ld5);

      map_gauge_s1->Fill(lscan[5][j],lscan[6][j], gd1);
      map_gauge_s2->Fill(lscan[5][j],lscan[6][j], gd2);
//      map_gauge_s3->Fill(lscan[5][j],lscan[6][j], gd3);
//      map_gauge_s4->Fill(lscan[5][j],lscan[6][j], gd4);
//    map_gauge_s5->Fill(lscan[5][j],lscan[6][j], gd5);

      distz_laser_s1->Fill(ld1);
      distz_laser_s2->Fill(ld2);
//      distz_laser_s3->Fill(ld3);
//      distz_laser_s4->Fill(ld4);
//    distz_laser_s5->Fill(ld5);

      distz_gauge_s1->Fill(gd1);
      distz_gauge_s2->Fill(gd2);
//      distz_gauge_s3->Fill(gd3);
//      distz_gauge_s4->Fill(gd4);
//    distz_gauge_s5->Fill(gd5);

      mean_laser_map->Fill(lscan[5][j],lscan[6][j], lmean2);
      mean_laser_distrib->Fill(lmean2);

      mean_gauge_map->Fill(lscan[5][j],lscan[6][j], gmean2);
      mean_gauge_distrib->Fill(gmean2);

      ldz1_mean = (lscan[0][j] - lmean2);
      ldz2_mean = (lscan[1][j] - lmean2);
//      ldz3_mean = (lscan[2][j] - lmean2);
//      ldz4_mean = (lscan[3][j] - lmean2);
//    ldz5_mean = (lscan[4][j] - lmean2);

      gdz1_mean = (gscan[0][j] - gmean2);
      gdz2_mean = (gscan[1][j] - gmean2);
//      gdz3_mean = (gscan[2][j] - gmean2);
//      gdz4_mean = (gscan[3][j] - gmean2);
//    gdz5_mean = (gscan[4][j] - gmean2);


      lstd = sqrt((ldz1_mean*ldz1_mean+ldz2_mean*ldz2_mean+ldz3_mean*ldz3_mean+ldz4_mean*ldz4_mean)/3);
      std_laser_distrib->Fill(lstd);
      std_map_laser->Fill(lscan[5][j],lscan[6][j],lstd);

      gstd = sqrt((gdz1_mean*gdz1_mean+gdz2_mean*gdz2_mean+gdz3_mean*gdz3_mean+gdz4_mean*gdz4_mean)/3);
      std_gauge_distrib->Fill(gstd);
      std_map_gauge->Fill(lscan[5][j],lscan[6][j],gstd);
   }


 // create a root file for the histograms;
  ostringstream rootfile;
  rootfile  << outpath <<"mean_and_rep_plates.root";
  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;

  ostringstream Mean_laser_map;
  Mean_laser_map << outpath << "/mean_laser_map.png";

  ostringstream Mean_laser_distrib;
  Mean_laser_distrib << outpath << "/mean_laser_distrib.png";

  ostringstream Std_laser_map;
  Std_laser_map << outpath << "/std_laser_map.png";

  ostringstream Std_laser_distrib;
  Std_laser_distrib << outpath << "/std_laser_distrib.png";

  ostringstream Mean_gauge_map;
  Mean_gauge_map << outpath << "/mean_gauge_map.png";

  ostringstream Mean_gauge_distrib;
  Mean_gauge_distrib << outpath << "/mean_gauge_distrib.png";

  ostringstream Std_gauge_map;
  Std_gauge_map << outpath << "/std_gauge_map.png";

  ostringstream Std_gauge_distrib;
  Std_gauge_distrib << outpath << "/std_gauge_distrib.png";


   TCanvas *c0 = new TCanvas("c0","c0",1000,700);
   map_laser_s1->SetMarkerSize(0.8);
   map_laser_s1->Draw("colz1,text45");
   map_laser_s1->SetStats(kFALSE);
   map_laser_s1->Write();

   TCanvas *c1 = new TCanvas("c1","c1",1000,700);
   map_laser_s2->SetMarkerSize(0.8);
   map_laser_s2->Draw("colz1,text45");
   map_laser_s2->SetStats(kFALSE);
   map_laser_s2->Write();
/*
   TCanvas *c2 = new TCanvas("c2","c2",1000,700);
   map_laser_s3->SetMarkerSize(0.8);
   map_laser_s3->Draw("colz1,text45");
   map_laser_s3->SetStats(kFALSE);
   map_laser_s3->Write();

   TCanvas *c3 = new TCanvas("c3","c3",1000,700);
   map_laser_s4->SetMarkerSize(0.8);
   map_laser_s4->Draw("colz1,text45");
   map_laser_s4->SetStats(kFALSE);
   map_laser_s4->Write();

  
   TCanvas *c4 = new TCanvas("c4","c4",1000,700);
   map_laser_s5->SetMarkerSize(0.8);
   map_laser_s5->Draw("colz1,text45");
   map_laser_s5->SetStats(kFALSE);
   map_laser_s5->Write();
*/

   TCanvas *gc0 = new TCanvas("gc0","gc0",1000,700);
   map_gauge_s1->SetMarkerSize(0.8);
   map_gauge_s1->Draw("colz1,text45");
   map_gauge_s1->SetStats(kFALSE);
   map_gauge_s1->Write();

   TCanvas *gc1 = new TCanvas("gc1","gc1",1000,700);
   map_gauge_s2->SetMarkerSize(0.8);
   map_gauge_s2->Draw("colz1,text45");
   map_gauge_s2->SetStats(kFALSE);
   map_gauge_s2->Write();
/*
   TCanvas *gc2 = new TCanvas("gc2","gc2",1000,700);
   map_gauge_s3->SetMarkerSize(0.8);
   map_gauge_s3->Draw("colz1,text45");
   map_gauge_s3->SetStats(kFALSE);
   map_gauge_s3->Write();

   TCanvas *gc3 = new TCanvas("gc3","gc3",1000,700);
   map_gauge_s4->SetMarkerSize(0.8);
   map_gauge_s4->Draw("colz1,text45");
   map_gauge_s4->SetStats(kFALSE);
   map_gauge_s4->Write();

  
   TCanvas *gc4 = new TCanvas("gc4","gc4",1000,700);
   map_gauge_s5->SetMarkerSize(0.8);
   map_gauge_s5->Draw("colz1,text45");
   map_gauge_s5->SetStats(kFALSE);
   map_gauge_s5->Write();
*/


   TCanvas *c21 = new TCanvas("c21","c21",1000,700);
   distz_laser_s1->Draw();
   distz_laser_s1->Write();

   TCanvas *c22 = new TCanvas("c22","c22",1000,700);
   distz_laser_s2->Draw();
   distz_laser_s2->Write();
/*
   TCanvas *c23 = new TCanvas("c23","c23",1000,700);
   distz_laser_s3->Draw();
   distz_laser_s3->Write();
   
   TCanvas *c24 = new TCanvas("c24","c24",1000,700);
   distz_laser_s4->Draw();
   distz_laser_s4->Write();

   TCanvas *c25 = new TCanvas("c25","c25",1000,700);
   distz_laser_s5->Draw();
   distz_laser_s5->Write();
*/

   TCanvas *gc21 = new TCanvas("gc21","gc21",1000,700);
   distz_gauge_s1->Draw();
   distz_gauge_s1->Write();

   TCanvas *gc22 = new TCanvas("gc22","gc22",1000,700);
   distz_gauge_s2->Draw();
   distz_gauge_s2->Write();
/*
   TCanvas *gc23 = new TCanvas("gc23","gc23",1000,700);
   distz_gauge_s3->Draw();
   distz_gauge_s3->Write();
   
   TCanvas *gc24 = new TCanvas("gc24","gc24",1000,700);
   distz_gauge_s4->Draw();
   distz_gauge_s4->Write();

   TCanvas *gc25 = new TCanvas("gc25","gc25",1000,700);
   distz_gauge_s5->Draw();
   distz_gauge_s5->Write();
*/


   TCanvas *cc2 = new TCanvas("cc2","cc2",1000,700);  
   mean_laser_map->SetMarkerSize(0.8);
   mean_laser_map->SetStats(kFALSE);
   mean_laser_map->Draw("colz1,text45"); 
   mean_laser_map->Write();
   cc2->Print(Mean_laser_map.str().c_str());

   TCanvas *cc3 = new TCanvas("cc3","cc3",1000,700); 
   mean_laser_distrib->Draw(); 
   mean_laser_distrib->Write();
   cc3->Print(Mean_laser_distrib.str().c_str());

   TCanvas *gcc2 = new TCanvas("gcc2","gcc2",1000,700);  
   mean_gauge_map->SetMarkerSize(0.8);
   mean_gauge_map->SetStats(kFALSE);
   mean_gauge_map->Draw("colz1,text45"); 
   mean_gauge_map->Write();
   gcc2->Print(Mean_gauge_map.str().c_str());

   TCanvas *gcc3 = new TCanvas("gcc3","gcc3",1000,700); 
   mean_gauge_distrib->Draw(); 
   mean_gauge_distrib->Write();
   gcc3->Print(Mean_gauge_distrib.str().c_str());

   TCanvas *c16 = new TCanvas("c16","c16",1000,700);  
   std_map_laser->SetMarkerSize(0.8);
   std_map_laser->SetStats(kFALSE);
   std_map_laser->Draw("colz1,text0"); 
   std_map_laser->Write();
   c16->Print(Std_laser_map.str().c_str());

   TCanvas *c26 = new TCanvas("c26","c26",1000,700);  
   std_laser_distrib->Draw(); 
   std_laser_distrib->Write();
   c26->Print(Std_laser_distrib.str().c_str());

   TCanvas *gc16 = new TCanvas("gc16","gc16",1000,700);  
   std_map_gauge->SetMarkerSize(0.8);
   std_map_gauge->SetStats(kFALSE);
   std_map_gauge->Draw("colz1,text0"); 
   std_map_gauge->Write();
   gc16->Print(Std_gauge_map.str().c_str());

   TCanvas *gc26 = new TCanvas("gc26","gc26",1000,700);  
   std_gauge_distrib->Draw(); 
   std_gauge_distrib->Write();
   gc26->Print(Std_gauge_distrib.str().c_str());

  myroot.Close();

   return; 
}





















