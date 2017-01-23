#include <stdio.h>
#include <string>
#include <vector>
//#include "AtlasStyle.C"

#define inpath "/home/atlas/Micromegas/M05Data/honeycomb/"
#define outpath "/home/atlas/Micromegas/M05Data/root_plot/honeycomb/"

void planarity_honeycomb_micrometer (string scan){

  // style option
   gROOT->LoadMacro("AtlasUtils.C");   // myText
   //SetAtlasStyle(); 
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".3f"); 
   gStyle->SetOptTitle(0);
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


  // file containing the data
  ostringstream file1;
  file1 << inpath  << scan ;  //compongo nome del file 
  scan.erase(scan.end()-4,scan.end());

  // histograms binning info
  int nxbin=60;
  int nybin=34;
  float xmin = -40;
  float xmax = 2280;
  float ymin = -40;
  float ymax =1320;

  // create all the histograms


  //  2d map of the table surface
  TH2F *tmap= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
  tmap->GetXaxis()->SetTitle("X (mm)");
  tmap->GetYaxis()->SetTitle("Y (mm)");
  tmap->GetZaxis()->SetTitle("Z (mm)");

  TH1F *thick = new TH1F("thickness","thickness",  30, 10.0, 10.3 ); 
  thick->GetXaxis()->SetTitle("mm");
  thick->GetYaxis()->SetTitle("counts");

  cout << "histos created " << endl;


  //variables for reading the file
  Float_t x,y, z;

 // reading the file
  ifstream in(file1.str().c_str());
  if (!in || in.bad()) return; // sanity check

  // use this variable to count the points
  int j=0;


  cout << "starting reading file " << endl;

  // looping on the file lines
  while (1) {

    // actual reading
    in >> x >> y >> z;
    if (!in.good()) break; // another sanity check 
	

    // filling the histos
    thick->Fill(z);

    // filling the 2d-map
    tmap->Fill(x,y, z);

    // increase at each loop, counts the lines
    j++;

  } // end while

  cout << "file completed "<< endl;


 // close the input file
  in.close();

 // create a root file for the histograms; 
  ostringstream rootfile;
  rootfile  << outpath << "/"<< scan <<".root";  
  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;


  ostringstream Smap;
  Smap << outpath << "/map_"<< scan <<".png";  

  ostringstream Sdistrib;
  Sdistrib  << outpath << "/distrib_"<< scan <<".png";  


  // create a canvas for each plot and drawing with different options
  TCanvas *c0 = new TCanvas("c0", "c0", 1000,700);
  tmap->SetMarkerSize(0.8);
  tmap->Draw("colz,text45");
  tmap->SetStats(kFALSE);
  tmap->Write();
  c0->Print(Smap.str().c_str());

  
  TCanvas *c22 = new TCanvas();
  thick->SetFillColor(kPink);
  thick->Draw();
  thick->Write();
  c22->Print(Sdistrib.str().c_str());


  myroot.Close();


  return;
}
