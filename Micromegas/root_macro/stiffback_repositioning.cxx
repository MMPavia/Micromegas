#include <stdio.h>
#include <string>
#include <vector>
//#include "AtlasStyle.C"

#define inpath "/home/atlas/Desktop/LWDAQ/MM_Data/"
#define outpath "/home/atlas/Micromegas/M05Data/root_plot/CCCD/"

void stiffback_repositioning (string scan){

  // style option
   gROOT->LoadMacro("AtlasUtils.C");   // myText
   gStyle->SetOptStat(111111);

  ostringstream file1;
  file1 << inpath  << scan ;  //compongo nome del file 
  scan.erase(scan.end()-4,scan.end());

  //variables for reading the file
  Float_t x,y,z;

 // reading the file
  ifstream in(file1.str().c_str());
  if (!in || in.bad()) return; // sanity check

  Float_t xmin = 999999; 
  Float_t ymin = 999999; 
  Float_t zmin = 999999; 
  Float_t xmax = -999999; 
  Float_t ymax = -999999; 
  Float_t zmax = -999999; 
  Float_t xmean=0; 
  Float_t ymean=0; 
  Float_t zmean=0; 

  cout << "starting reading file " << endl;
  int j=0; 
  // looping on the file lines
  while (1) {
    in >> x >> y >> z;
    if (!in.good()) break; // another sanity check //esco dal loop quando finisco le righe
    
    if(x < xmin) xmin=x;
    if(y < ymin) ymin=y;
    if(z < zmin) zmin=z;
    if(x > xmin) xmax=x;
    if(y > ymin) ymax=y;
    if(z > zmin) zmax=z;
	
    xmean +=x;
    ymean +=y;
    zmean +=z;
 j++;	
 
  } // end while
 in.close(); 
    xmean=xmean/j; 
    ymean=ymean/j; 
    zmean=zmean/j; 


string namex= scan + "x"; 
string namey= scan + "y"; 
string namez= scan + "z"; 

xbin = xmax-xmin; 
ybin = ymax-ymin; 
zbin = zmax-zmin; 


  TH1F *hx = new TH1F(namex.c_str(), namex.c_str(),xbin+20, -xbin/2-10, xbin/2+10 ); 
  hx->GetXaxis()->SetTitle("x (#mum)"); 

  TH1F *hy = new TH1F(namey.c_str(), namey.c_str(), ybin+20, -ybin/2-10, ybin/2+10 ); 
  hy->GetXaxis()->SetTitle("y (#mum)"); 

  TH1F *hz = new TH1F(namez.c_str(), namez.c_str(),zbin+10, -zbin/2-5, zbin/2+5 ); 
  hz->GetXaxis()->SetTitle("z (#mum)"); 

  cout << "histos created " << endl;

  cout << "file completed "<< endl;

  ifstream in2(file1.str().c_str());
  if (!in2 || in2.bad()) return; // sanity check
  while (1) {

    // actual reading
    in2 >> x >> y >> z;
    cout << x << " " << y << " "<< z<< endl; 
    if (!in2.good()) break; // another sanity check //esco dal loop quando finisco le righe
 
	
    // filling the histos
    hx->Fill(x-xmean); 
    hy->Fill(y-ymean); 
    hz->Fill(z-zmean); 

    // increase at each loop, counts the lines
    j++;

  } // end while

 // close the input file
  in2.close();

    TCanvas *c1 = new TCanvas(); 
hx->Draw();
    TCanvas *c2 = new TCanvas(); 
hy->Draw();
    TCanvas *c3 = new TCanvas(); 
hz->Draw();



  // raw values: average and differences wrt to the average
  // Float_t value = z_corr->GetMean();
  // Float_t sigma = z_corr->GetRMS();
/*
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
*/

  return;
}
