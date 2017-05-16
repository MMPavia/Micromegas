#include <stdio.h>
#include <string>
#include <vector>
//#include "AtlasStyle.C"

#define inpath "/home/atlas/Micromegas/M05Data/mapping/"
#define outpath "/home/atlas/Micromegas/M05Data/root_plot/modulo05/"

void planarity_plates (bool plates, string scan){

  // style option
  // gROOT->LoadMacro("AtlasUtils.C");   // myText
   //SetAtlasStyle(); 
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".3f"); //approssimazione a tre cifre decimali sulla mappa
   gStyle->SetOptTitle(0);
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


  // file containing the data
  ostringstream file1;
  file1 << inpath  << scan ;  //compongo nome del file   file1 << path  << scan ;
  scan.erase(scan.end()-4,scan.end());

  // histograms binning info
  //int nxbin=28;
  int nxbin=24;
  int nybin=30;
  float xmin = 253/10;
  float xmax = 2414/10;
  float ymax = 1239;
  float ymin = 162;
   
   int ndistbin = 30;
   float xdistmin = 11.4;
   float xdistmax = 11.9;

   if(!plates){
        ndistbin = 50;
        xdistmin = 700;
        xdistmax = 900;
   }


  // create all the histograms


  //  2d map of the table surface
  TH2F *tmap= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
  tmap->GetXaxis()->SetTitle("X (cm)");
  tmap->GetYaxis()->SetTitle("Y (mm)");
  tmap->GetZaxis()->SetTitle("Z (#mum)");


  // optical line value distribution
  TH1F *optline = new TH1F("optline_distrib","optline_distrib", 80, -0.1, 0.1 );   //linea ottica 
  optline->GetXaxis()->SetTitle("mm");
  optline->GetYaxis()->SetTitle("counts");

  // indicator raw measurements
  TH1F *z_raw = new TH1F("z_raw","z_raw", ndistbin, xdistmin, xdistmax);
  z_raw->GetXaxis()->SetTitle("mm");
  z_raw->GetYaxis()->SetTitle("counts");

  // z corrected value ( indicator - optical line ) 
  //TH1F *z_corr = new TH1F("z_corr","z_corr",  80, -0.5, 0.5 ); // z= diff tra tastatore e ottico
  TH1F *z_corr = new TH1F("z_corr","z_corr",  ndistbin, xdistmin, xdistmax  ); // z= diff tra tastatore e ottico
  z_corr->GetXaxis()->SetTitle("mm");
  z_corr->GetYaxis()->SetTitle("counts");

  // temperature distribution 
  TH1F *temperature1 = new TH1F ("temperature1", "temperature1", 40, 19., 23);
  TH1F *temperature2 = new TH1F ("temperature2", "temperature2", 40, 19., 23);


  cout << "histos created " << endl;


  Float_t x,y, opt, laser, temp1, temp2, coord;

  // reading the file
  ifstream in(file1.str().c_str());
  if (!in || in.bad()) return; // sanity check

  // use this variable to count the points
  int j=0;


  cout << "starting reading file " << endl;

  // looping on the file lines
  while (1) {

    in >> x >> y >> opt >> laser >> temp1 >> temp2 ;
    if (!in.good()) break; 
	
    coord = laser - opt; // definsco z
    cout << x <<  " " << -y << " coord "  << coord  << endl;  

    // filling the histos
    optline->Fill(opt); // metto in mu
    z_raw->Fill(laser);
    // z_corr->Fill(coord*1000);
    z_corr->Fill(coord);
    temperature1->Fill(temp1);
    temperature2->Fill(temp2);

    // filling the 2d-map
    tmap->Fill(x/10,-y, coord);

    // increase at each loop, counts the lines
    j++;

  } // end while

  cout << "file completed "<< endl;


  // controllo per buchi
   Int_t binx = tmap->GetNbinsX();
   Int_t biny = tmap->GetNbinsY();

   for (int j=1; j<=binx; j++){
      for (int i=1; i<=biny; i++){
 
	double Z = tmap->GetBinContent(j,i);
      }
     }

 // close the input file
  in.close();

  // raw values: average and differences wrt to the average
  Float_t value = z_corr->GetMean();
  Float_t sigma = z_corr->GetRMS();


 // create a root file for the histograms; 
  ostringstream rootfile;
  rootfile  << outpath << "/" << scan <<".root";  
  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;


  ostringstream Smap;
  Smap << outpath << "/map_"<< scan <<".png";  

  ostringstream Smapdiff;
  Smapdiff << outpath << "/map_diffmean_"<< scan <<".png";  

  ostringstream Sdistrib;
  Sdistrib  << outpath << "/distrib_"<< scan <<".png";  



  // create a canvas for each plot and drawing with different options
  TCanvas *c0 = new TCanvas("c0", "c0", 1000,700);
  tmap->SetMarkerSize(0.8);
  tmap->Draw("colz,text45");
  tmap->SetStats(kFALSE);
  tmap->GetZaxis()->SetRangeUser(11.4, 11.780);
  tmap->Write();
  // c0->Print(Smap.str().c_str());

  TCanvas *c22 = new TCanvas();
  z_corr->Draw();
  z_corr->Write();
  c22->Print(Sdistrib.str().c_str());

  myroot.Close();


  return;
}
