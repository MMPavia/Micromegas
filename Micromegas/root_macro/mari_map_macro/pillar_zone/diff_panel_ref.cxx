#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>

//#include "AtlasStyle.C"


//macro che ha in pancia i root file che arrivano da repeat&mean 
//e vuole in input i txt della diag sx e dx in ordine


#define inpath1 "/home/atlas/Micromegas/M05Data/mapping/"
#define inpath "/home/atlas/Micromegas/M05Data/root_plot/"
#define outpath "/home/atlas/Micromegas/M05Data/root_plot/"
//#define inpath "/Users/mariacristinalopresti/Desktop/mm/gauge-laser/pcb_zone/"
//#define outpath "/Users/mariacristinalopresti/Desktop/mm/gauge-laser/pcb_zone/"

//q=0,1 ->laser,gauge

void diff_panel_ref (int q, string scan){

  // style option
   //gROOT->LoadMacro("AtlasUtils.C");   // myText
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".3f");
   //gStyle->SetOptTitle(0);
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


// file containing the data.txt
   ostringstream file1;
   file1 << inpath1  << scan;
   scan.erase(scan.end()-4,scan.end());
   scan.erase(scan.begin(),scan.begin()+13);

// reference files
   ostringstream file2;

            if(q==1){
                      file2 << inpath<< "mean_and_rep_plates_gauge_ref.root";    //q=1 gauge
            }
	        if(q==0){
                      file2 << inpath<< "mean_and_rep_plates_laser_ref.root";    //q=0 laser
            }
   

   TFile *f1 = new TFile(file2.str().c_str());
   TH2F *ref_s = (TH2F*)f1->Get("mean_map");
   TH2F *ave_s = (TH2F*)ref_s->Clone("average");
   ave_s->SetTitle("Average Reference Map");
   ave_s->Scale(0.001);
   
   int nxbin=24;   // 
   int nybin=30;   //
   float xmin = 253/10;
   float xmax = 2414/10;
   float ymax = 1239;
   float ymin = 162;
    
   TH2F *map_s = new TH2F("map_s","map_s", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s->GetXaxis()->SetTitle("X (cm)");
   map_s->GetYaxis()->SetTitle("Y (mm)");
   map_s->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *diff = new TH2F("diff","diff", nxbin, xmin, xmax, nybin, ymin, ymax );
   diff->GetXaxis()->SetTitle("X (cm)");
   diff->GetYaxis()->SetTitle("Y (mm)");
   diff->GetZaxis()->SetTitle("Z (#mu m)");

   int ndistbin = 100; 
   float xdistmin =10; 
   float xdistmax =12; 

   TH1F *distz_s = new TH1F("distz_s","distz_s", ndistbin,xdistmin, xdistmax  ); 
   distz_s->GetXaxis()->SetTitle("#mu ");
   distz_s->GetYaxis()->SetTitle("counts");
   distz_s->SetFillColor(2);

   TH1F *distz_diff = new TH1F("distz_diff","distz_diff",  ndistbin,xdistmin, xdistmax  );
   distz_diff->GetXaxis()->SetTitle("#mu ");
   distz_diff->GetYaxis()->SetTitle("counts");
   distz_diff->SetFillColor(2);
      


   Int_t j=0; 

   Float_t x,y, opt, laser, temp1, temp2, coord;



// leggo file e riempio array con coord

    ifstream in1(file1.str().c_str());
    if (!in1 || in1.bad()) return;

    while(1){
      
       in1 >> x >> y >> opt >> laser >>  temp1 >> temp2;
	   if(!in1.good()) break; 

	   coord = (laser - opt);
	   //scan[0][j]=coord*1000;
	   //m1 += scan[0][j];
	   cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " << temp2 << "c1=  " << coord << " "  << endl;
	   distz_s->Fill(coord);
	   
	   if(q==1){
           map_s->Fill((x+40.0)/10,-y, coord);
	       }
	   if(q==0){ 
           map_s->Fill(x/10,-y, coord);
           }
	   j++;
    }

    diff->Add(ave_s,map_s,-1,+1);

   float zref,zmap, z; 
    
   for (int i=1; i<=nybin; i++){
      for (int j=1; j<=nxbin; j++){
	  zref = ave_s->GetBinContent(j,i);
	  zmap = map_s->GetBinContent(j,i);
	  z = diff->GetBinContent(j,i);
	  if(z!=0) {
	     distz_diff->Fill(z);
	     cout << zref << " " << zmap << " z=  " << z << endl;
          }
      }
    }
  Double_t media, RMS;


 // create a root file for the histograms;
  ostringstream rootfile;
  rootfile  << outpath <<"/diff_pan_plate_" << scan << ".root"; 
  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;


 // output data file
  ofstream Data;
  ostringstream textfile;
  textfile << outpath << scan <<".dat"; 
  Data.open( textfile.str().c_str(), ofstream::out | ofstream::app);
  //TFile mytext(textfile.str().c_str(), "RECREATE");
  cout << "Text output file: " << endl << textfile.str() << endl;

    ostringstream mapcorr1;
    mapcorr1 << outpath << "/diff_pan_plate_" << scan <<".png";

    ostringstream zcorr;
    zcorr << outpath << "/diff_pan_plate_zdistrib_" << scan << ".png";

   TCanvas *c0 = new TCanvas("map_s","map_s",1000,700);
   map_s->SetMarkerSize(0.8);
   map_s->Draw("colz1,text45");
   map_s->SetStats(kFALSE);
   map_s->GetZaxis()->SetRangeUser(11.4, 11.7);
   map_s->Write();

   TCanvas *c2 = new TCanvas("ref_s","ref_s",1000,700);
   ref_s->SetMarkerSize(0.8);
   ref_s->Draw("colz1,text45");
   ref_s->SetStats(kFALSE);
   ref_s->Write();

   TCanvas *c4 = new TCanvas("diff","diff",1000,700);
   diff->SetMarkerSize(0.8);
   diff->Draw("colz1,text45");
   diff->SetStats(kFALSE);
   diff->GetZaxis()->SetRangeUser(11.4, 11.7);
   diff->Write();
   c4->Write(mapcorr1.str().c_str());
    media=distz_diff->GetMean(1);
   RMS=distz_diff->GetRMS(1);
   //min=distz_diff->GetMinimum(1);
   //max=distz_diff->GetMaximum(1);
   //Data << media  << " \t " << RMS << "\t" << max << "\t" << min << endl;
   Data <<"mean= " <<  media  <<"\t" << "RMS= " << RMS << endl;
   TCanvas *c21 = new TCanvas("c21","c21",1000,700);
   distz_s->Draw();
   distz_s->Write();

   TCanvas *c23 = new TCanvas("c23","c23",1000,700);
   distz_diff->Draw();
   distz_diff->Write();   
   c23->Write(zcorr.str().c_str());   

  myroot.Close();


   return; 
}




















