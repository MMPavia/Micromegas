#include <stdio.h>
#include <string>
#include <vector>
//#include "AtlasStyle.C"

#define inpath "/home/atlas/Micromegas/M05Data/mapping/"
#define outpath "/home/atlas/Micromegas/M05Data/root_plot/"

void planarity_plates (string scan){

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
  int nxbin=28;
  int nybin=40;
  float xmin = 191;
  float xmax = 2422;
  float ymax = 1258;
  float ymin = 130;

  // create all the histograms


  //  2d map of the table surface
  TH2F *tmap= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
  tmap->GetXaxis()->SetTitle("X (mm)");
  tmap->GetYaxis()->SetTitle("Y (mm)");
  tmap->GetZaxis()->SetTitle("Z (#mum)");


  // optical line value distribution
  TH1F *optline = new TH1F("optline_distrib","optline_distrib", 80, 0.9, 1 );   //linea ottica 
  optline->GetXaxis()->SetTitle("mm");
  optline->GetYaxis()->SetTitle("counts");

  // indicator raw measurements
  TH1F *z_raw = new TH1F("z_raw","z_raw", 80, -0.3, 0.1);
  z_raw->GetXaxis()->SetTitle("mm");
  z_raw->GetYaxis()->SetTitle("counts");

  // z corrected value ( indicator - optical line ) 
  TH1F *z_corr = new TH1F("z_corr","z_corr",  80, -1.1, -0.85 ); // z= diff tra tastatore e ottico
  z_corr->GetXaxis()->SetTitle("mm");
  z_corr->GetYaxis()->SetTitle("counts");

  // temperature distribution 
  TH1F *temperature1 = new TH1F ("temperature1", "temperature1", 40, 19., 23);
  TH1F *temperature2 = new TH1F ("temperature2", "temperature2", 40, 19., 23);


  cout << "histos created " << endl;


  //variables for reading the file
  Float_t x,y, opt, laser, temp1, temp2, coord;
  Float_t coord_old,coordmean,yold;

 // reading the file
  ifstream in(file1.str().c_str());
  if (!in || in.bad()) return; // sanity check

  // use this variable to count the points
  int j=0;


  cout << "starting reading file " << endl;

  // looping on the file lines
  while (1) {

    in >> x >> y >> opt >> laser ;
    // another sanity check //esco dal loop quando finisco le righe
    if (!in.good()) break; 
	
    coord = laser - opt; // definsco z
    cout << x <<  " " << -y << " " << coord  << endl;  

    // filling the histos
    optline->Fill(opt); // metto in mu
    z_raw->Fill(laser);
    z_corr->Fill(coord);
//    temperature1->Fill(temp1);
//    temperature2->Fill(temp2);

    // filling the 2d-map
    if(abs(y-yold)< 28) 
    tmap->Fill(x,-y, coord*1000);

    // increase at each loop, counts the lines
    j++;
    yold = y;

  } // end while

  cout << "file completed "<< endl;


 // controllo per buchi
   Int_t binx = tmap->GetNbinsX();
   Int_t biny = tmap->GetNbinsY();

   for (int i=1; i<=biny; i++){
      for (int j=1; j<=binx; j++){
 
	double Z = tmap->GetBinContent(j,i);

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
  tmap->Draw("colz1,text45");
  tmap->SetStats(kFALSE);
  tmap->Write();
  c0->Print(Smap.str().c_str());


  TCanvas *c1 = new TCanvas();
  c1->Divide(1,2);
  c1->cd(1); 
  optline->Draw();
  optline->Write();
  c1->cd(2); 
  z_raw->Draw();
  z_raw->Write();
  
  TCanvas *c22 = new TCanvas();
  z_corr->Draw();
  z_corr->Write();
  c22->Print(Sdistrib.str().c_str());

  TCanvas *c3 = new TCanvas();
  c3->Divide(1,2);
  c3->cd(1); 
  temperature1->Draw();
  temperature1->Write();
  c3->cd(2); 
  temperature2->Draw();
  temperature2->Write();


  myroot.Close();


  return;
}
