#include <stdio.h>
#include <string>
#include <vector>
//#include "AtlasStyle.C"

#define inpath "/home/atlas/Micromegas/M05Data/pressure/"
#define outpath "/home/atlas/Micromegas/M05Data/root_plot/pressure/"

void plot_press (string scan, float xmin, float xmax){

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
  file1 << inpath  << scan ;  //compongo nome del file 
  scan.erase(scan.end()-4,scan.end());


  // create all the histograms

//  int bin = (xmax-xmin)*10/2;
  int bin = (xmax-xmin)/2;


  TH1F *press = new TH1F("pressure_distrib","pressure_distrib", bin, xmin, xmax );   //linea ottica 
  press->GetXaxis()->SetTitle("pressure (mb)");

  TH1F *press_sb = new TH1F("stiffback pressure distrib","stiffback pressure distrib", bin, xmin, xmax );   //linea ottica 
  press_sb->GetXaxis()->SetTitle("pressure (mb)");

  cout << "histos created " << endl;


  //variables for reading the file
  TString date;
  Float_t ptab, pstiff;

 // reading the file
  ifstream in(file1.str().c_str());
  if (!in || in.bad()) return; // sanity check

  // use this variable to count the points
  int j=0;


  cout << "starting reading file " << endl;

  // looping on the file lines
  while (1) {

    // actual reading
    in >> date >> ptab >> pstiff;
    if (!in.good()) break; // another sanity check //esco dal loop quando finisco le righe
	
    // filling the histos
    press->Fill(ptab*1000.); // metto in mu
    press_sb->Fill(pstiff*1000);

    // increase at each loop, counts the lines
    j++;

  } // end while

  cout << "file completed "<< endl;

 // close the input file
  in.close();

  // raw values: average and differences wrt to the average
  // Float_t value = z_corr->GetMean();
  // Float_t sigma = z_corr->GetRMS();

  ostringstream SdistribTB;
  SdistribTB  << outpath << "/distrib_tb_"<< scan <<".png";  

  ostringstream SdistribSB;
  SdistribSB  << outpath << "/distrib_sb_"<< scan <<".png";  
  
  TCanvas *c22 = new TCanvas();
  press->SetFillColor(kAzure-3);
  press->Draw();
  c22->Print(SdistribTB.str().c_str());

  TCanvas *c2 = new TCanvas();
  press_sb->SetFillColor(kRed-3);
  press_sb->Draw();
  c2->Print(SdistribSB.str().c_str());


  return;
}
