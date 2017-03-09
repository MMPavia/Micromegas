#include <stdio.h>
#include <string>
#include <vector>
//#include "AtlasStyle.C"

//#define inpath "/home/atlas/Micromegas/M05Data/mapping/"
//#define outpath "/home/atlas/Micromegas/M05Data/root_plot/"

#define inpath "/home/atlas/Micromegas/M05Data/mapping/" 
#define outpath "/home/atlas/Micromegas/M05Data/root_plot/"


void planarity_plates (bool plates, string scan){

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
  file1 << inpath  << scan ;  //compongo nome del file   file1 << path  << scan ;
  scan.erase(scan.end()-4,scan.end());

  // histograms binning info
  //int nxbin=28;
  int nxbin=5;
  int nybin=14;
  float xmin = 258/10;
  float xmax = 656/10;
  float ymax = 1258;
  float ymin = 130;

   int ndistbin = 40;
   float xdistmin = -500;
   float xdistmax = 500;

   if(!plates){
       ndistbin = 50;
       //xdistmin = 650;
       //xdistmax = 900;
       xdistmin = -500;
       xdistmax = 500;
    }

  // create all the histograms


  //  2d map of the table surface
  TH2F *tmap_laser= new TH2F("map_laser","map_laser", nxbin, xmin, xmax, nybin, ymin, ymax );
  tmap_laser->GetXaxis()->SetTitle("X (cm)");
  tmap_laser->GetYaxis()->SetTitle("Y (mm)");
  tmap_laser->GetZaxis()->SetTitle("Z (#mum)");

  TH2F *tmap_gauge= new TH2F("map_gauge","map_gauge", nxbin, xmin, xmax, nybin, ymin, ymax );
  tmap_gauge->GetXaxis()->SetTitle("X (cm)");
  tmap_gauge->GetYaxis()->SetTitle("Y (mm)");
  tmap_gauge->GetZaxis()->SetTitle("Z (#mum)");

  // z corrected value ( indicator - optical line ) 
  TH1F *z_corr_laser = new TH1F("z_corr_laser","z_corr_laser",  ndistbin, xdistmin, xdistmax  ); 
  z_corr_laser->GetXaxis()->SetTitle("mm");
  z_corr_laser->GetYaxis()->SetTitle("counts");

  TH1F *z_corr_gauge = new TH1F("z_corr_gauge","z_corr_gauge",  ndistbin, xdistmin, xdistmax  ); 
  z_corr_gauge->GetXaxis()->SetTitle("mm");
  z_corr_gauge->GetYaxis()->SetTitle("counts");

  cout << "histos created " << endl;


  //variables for reading the file
  Float_t x,y, opt, laser, gauge, temp1, temp2, lcoord, gcoord;

  // reading the file
  ifstream in(file1.str().c_str());
  if (!in || in.bad()) return; // sanity check

  // use this variable to count the points
  int j=0;


  cout << "starting reading file " << endl;

  // looping on the file lines
  while (1) {

    in >> x >> y >> opt >> laser >> gauge >>  temp1 >> temp2 ;
    if (!in.good()) break; 
	
    lcoord = laser - opt; // definsco z
    gcoord = gauge - opt; // definsco z
    cout << x <<  " " << -y << " laser  "  << lcoord*1000  <<  " gauge  "  << gcoord*1000  << endl;  

    // filling the histos
    z_corr_laser->Fill(lcoord*1000);
    z_corr_gauge->Fill(gcoord*1000);

    // filling the 2d-map
    tmap_laser->Fill(x/10,-y, lcoord*1000);
    tmap_gauge->Fill(x/10,-y, gcoord*1000);

    // increase at each loop, counts the lines
    j++;

  } // end while

  cout << "file completed "<< endl;

/*
   // controllo per buchi
   Int_t binx = tmap->GetNbinsX();
   Int_t biny = tmap->GetNbinsY();

   for (int j=1; j<=binx; j++){
      for (int i=1; i<=biny; i++){
 
	double Z = tmap->GetBinContent(j,i);

	//cout << "binx " << j << " biny " << i << " Z  " <<  Z << endl;  

	//if (Z<-200) {

	//	double b = tmap->GetBinContent(j,i-2);
	//	double c = tmap->GetBinContent(j+1,i-1);
	//	double d = tmap->GetBinContent(j+1,i);
	//	double e = tmap->GetBinContent(j+1,i+1);
	//	double f = tmap->GetBinContent(j,i+2);
	//	double mean = (b+f)/2;
	//	tmap->SetBinContent(j,i,mean);

	//};

      }
   }
*/
 // close the input file
  in.close();


 // create a root file for the histograms; 
  ostringstream rootfile;
  rootfile  << outpath << "/" << scan <<".root";  
  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;


  ostringstream lSmap;
  lSmap << outpath << "/map_laser_"<< scan <<".png";  

  ostringstream lSdistrib;
  lSdistrib  << outpath << "/distrib_laser_"<< scan <<".png";  

  ostringstream gSmap;
  gSmap << outpath << "/map_gauge_"<< scan <<".png";  

  ostringstream gSdistrib;
  gSdistrib  << outpath << "/distrib_gauge_"<< scan <<".png";  


  // create a canvas for each plot and drawing with different options
  TCanvas *lc0 = new TCanvas("lc0", "lc0", 1000,700);
  tmap_laser->SetMarkerSize(0.8);
  tmap_laser->SetMaximum(-0.000001);
  tmap_laser->Draw("colz,text45");
  tmap_laser->SetStats(kFALSE);
  tmap_laser->Write();
  lc0->Print(lSmap.str().c_str());

  TCanvas *gc0 = new TCanvas("gc0", "gc0", 1000,700);
  tmap_gauge->SetMarkerSize(0.8);
  tmap_gauge->SetMaximum(-0.000001);
  tmap_gauge->Draw("colz,text45");
  tmap_gauge->SetStats(kFALSE);
  tmap_gauge->Write();
  gc0->Print(gSmap.str().c_str());
  
  TCanvas *lc22 = new TCanvas();
  z_corr_laser->Draw();
  z_corr_laser->Write();
  lc22->Print(lSdistrib.str().c_str());

  TCanvas *gc22 = new TCanvas();
  z_corr_gauge->Draw();
  z_corr_gauge->Write();
  gc22->Print(gSdistrib.str().c_str());

  myroot.Close();


  return;
}
