#include <stdio.h>
#include <string>
#include <vector>
//#include "AtlasStyle.C"

#define inpath "/home/atlas/Micromegas/M05Data/mapping/"
#define outpath "/home/atlas/Micromegas/M05Data/root_plot/"

//#define inpath "/Users/gabriella/cernbox/MM_Pavia/Data/Data2017/03_LaserMeasureStep_2017_01_30/"
//#define outpath "/Users/gabriella/cernbox/MM_Pavia/Data/Data2017/03_LaserMeasureStep_2017_01_30/"


void planarity_plates_gauge (bool plates, bool stiffback,  string scan){

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
  int nxbin=24;
  int nybin=30;
  float xmin = 253/10;
  float xmax = 2414/10;
  float ymax = 1239;
  float ymin = 162;

   int ndistbin = 50;
   float xdistmin = 600;
   float xdistmax = 900;

   if(!plates){
        ndistbin = 50;
        xdistmin = 1400;
        xdistmax = 1600;
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


  //variables for reading the file
  Float_t x,y, opt, gauge, temp1, temp2, coord;

  // reading the file
  ifstream in(file1.str().c_str());
  if (!in || in.bad()) return; // sanity check

  // use this variable to count the points
  int j=0;

  Float_t myzero=0; 



  cout << "starting reading file " << endl;

  // looping on the file lines
  while (1) {

    in >> x >> y >> opt >> gauge >> temp1 >> temp2 ;
    if (!in.good()) break; 
	
    coord = gauge - opt; // definsco z
    cout << x <<  " " << -y << " opt " << opt  << " gauge " << gauge << " coord "  << coord*1000  << endl;  

    if (stiffback && j==0) myzero = coord; 
    else if (!stiffback)  myzero=0; 


    // filling the histos
    optline->Fill(opt); // metto in mu
    z_raw->Fill(gauge);
    z_corr->Fill((coord-myzero)*1000);
    temperature1->Fill(temp1);
    temperature2->Fill(temp2);

    // filling the 2d-map
    tmap->Fill(x/10,-y, (coord-myzero)*1000);

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
  // tmap->SetMaximum(-0.000001);
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

/*
  TCanvas *c3 = new TCanvas();
  c3->Divide(1,2);
  c3->cd(1); 
  temperature1->Draw();
  temperature1->Write();
  c3->cd(2); 
  temperature2->Draw();
  temperature2->Write();
*/

  myroot.Close();


  return;
}
