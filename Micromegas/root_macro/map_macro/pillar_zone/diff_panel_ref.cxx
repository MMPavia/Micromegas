#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>

//#include "AtlasStyle.C"


//macro che ha i root file che arrivano da repeat&mean 
//e vuole in input i txt del run sul pannello

#define inpath1 "/home/atlas/Micromegas/ProdData/mapping/"
// GG: change for laser/gauge
#define inpath2 "/home/atlas/Micromegas/ProdData/reference/"
//#define inpath2 "/home/atlas/Micromegas/M05Data/reference/"
#define outpath "/home/atlas/Micromegas/ProdData/root_plot/"


void diff_panel_ref (string scan){

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
   // GG: change for laser/gauge
   // file2 << inpath2 << "mean_and_rep_plates_laser_ref_tab.root";
   file2 << inpath2 << "mean_and_rep_plates_laser_ref_tab_new.root";
        
   

   TFile *f1 = new TFile(file2.str().c_str());
   TH2F *ref_s = (TH2F*)f1->Get("mean_map");
   TH2F *ave_s = (TH2F*)ref_s->Clone("average");
   ave_s->SetTitle("Average Reference Map");
   ave_s->Scale(0.001);
   
   int nxbin=24;   // 
   int nybin=30;   //
   // GG: change for laser/gauge
   float xmin = 320/10;
   //float xmin = 230/10;
   float xmax = 2440/10;
   float ymax = 1230;
   float ymin = 200;
    
   TH2F *map_s = new TH2F("map_s","map_s", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s->GetXaxis()->SetTitle("X (cm)");
   map_s->GetYaxis()->SetTitle("Y (mm)");
   map_s->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *diff = new TH2F("diff","diff", nxbin, xmin, xmax, nybin, ymin, ymax );
   diff->GetXaxis()->SetTitle("X (cm)");
   diff->GetYaxis()->SetTitle("Y (mm)");
   diff->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *diffpillar = new TH2F("diffpillar","diffpillar", nxbin, xmin, xmax, nybin, ymin, ymax );
   diffpillar->GetXaxis()->SetTitle("X (cm)");
   diffpillar->GetYaxis()->SetTitle("Y (mm)");
   diffpillar->GetZaxis()->SetTitle("Z (#mu m)");

   int ndistbin = 50; 
   float xdistmin =11.5; 
   float xdistmax =11.9; 

   TH1F *distz_s = new TH1F("distz_s","distz_s", ndistbin,xdistmin, xdistmax  ); 
   distz_s->GetXaxis()->SetTitle("#mu ");
   distz_s->GetYaxis()->SetTitle("counts");
   distz_s->SetFillColor(2);

   TH1F *distz_diff = new TH1F("distz_diff","distz_diff",  ndistbin,xdistmin, xdistmax  );
   distz_diff->GetXaxis()->SetTitle("#mu ");
   distz_diff->GetYaxis()->SetTitle("counts");
   distz_diff->SetFillColor(2);
      

   TH1F *distz_pillar = new TH1F("distz_pillar","distz_pillar",  ndistbin,xdistmin, xdistmax  );
   distz_pillar->GetXaxis()->SetTitle("#mu ");
   distz_pillar->GetYaxis()->SetTitle("counts");
   distz_pillar->SetFillColor(2);
      

   Int_t j=0; 
   Int_t q=0, n=146;
   Float_t array[n];
   Float_t x,y, opt, laser, temp1, temp2, coord;
   Double_t media,stdev;
   Double_t minimo=14000;
   Double_t massimo=-14000;



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
           map_s->Fill(x/10,-y, coord);
	   j++;
    }

    diff->Add(ave_s,map_s,-1,+1);
    //diffpillar->Add(diff,pillar,+1,+1);

   float zref,zmap, z, zpillar; 
    
   for (int i=1; i<=nybin; i++){
      for (int j=1; j<=nxbin; j++){
	  zref = ave_s->GetBinContent(j,i);
	  zmap = map_s->GetBinContent(j,i);
	  z = diff->GetBinContent(j,i);
          zpillar = z + 0.128;
	  if(z!=0) {
	  array[j]=z;
	  q += array[j];
	  if(array[j]<minimo){minimo = array[j];}
          if(array[j]>massimo){massimo = array[j];}
	     distz_diff->Fill(z);
             distz_pillar->Fill(zpillar); 
	     cout << zref << " " << zmap << " z=  " << z << " " << zpillar << endl;
          }
      }
    }


   media =  distz_diff->GetMean();
   stdev =  distz_diff->GetRMS();


   cout << " media= " << media << " rms= " << stdev << " min= " << minimo << "max= " << massimo << endl;



 // create a root file for the histograms;
  ostringstream rootfile;
  rootfile  << outpath <<"/diff_pan_plate_" << scan << ".root"; 
  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;

  //create a txt data file;
  ofstream Data;
  ostringstream txtfile;
  txtfile << outpath <<"/summary_diff_pan_plate_" << scan << ".txt"; 

  Data.open(txtfile.str().c_str(), ofstream::out | ofstream::app);
  cout << "Text output file: " << endl << txtfile.str() << endl;
  Data << "Mean   rms   min   max" << "\n" << media << "\t" << stdev << "\t" << minimo << "\t" << massimo << endl;




    ostringstream mapcorr1;
    mapcorr1 << outpath << "/diff_pan_plate_" << scan <<".png";

    ostringstream mapcorr2;
    mapcorr2 << outpath << "/diff_pan_plate_pillar" << scan <<".png";

    ostringstream zcorr;
    zcorr << outpath << "/diff_pan_plate_zdistrib_" << scan << ".png";

    ostringstream zcorrpillar;
    zcorrpillar << outpath << "/diff_pan_plate_zdistrib_pillar" << scan << ".png";
 
  TCanvas *c0 = new TCanvas("map_s","map_s",1000,700);
   map_s->SetMarkerSize(0.8);
   map_s->Draw("colz1,text45");
   map_s->SetStats(kFALSE);
   map_s->GetZaxis()->SetRangeUser(11.5, 11.75);
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
   diff->GetZaxis()->SetRangeUser(11.5, 11.80);
   diff->Write();
   c4->Write(mapcorr1.str().c_str());

   /*TCanvas *c5 = new TCanvas("diffpillar","diffpillar",1000,700);
   diffpillar->SetMarkerSize(0.8);
   diffpillar->Draw("colz1,text45");
   diffpillar->SetStats(kFALSE);
   diffpillar->GetZaxis()->SetRangeUser(11.6, 11.80);
   diffpillar->Write();
   c5->Write(mapcorr2.str().c_str());
*/

   TCanvas *c21 = new TCanvas("c21","c21",1000,700);
   distz_s->Draw();
   distz_s->Write();

 //  TCanvas *c22 = new TCanvas("c22","c22",1000,700);
 //  zdist_ref_s->Draw();
 //  zdist_ref_s->Write();

   TCanvas *c23 = new TCanvas("c23","c23",1000,700);
   distz_diff->Draw();
   distz_diff->Write();   
   c23->Write(zcorr.str().c_str());   

   TCanvas *c24 = new TCanvas("c24","c24",1000,700);
   distz_pillar->Draw();
   distz_pillar->Write();   
   c24->Write(zcorrpillar.str().c_str());   

  myroot.Close();


   return; 
}


