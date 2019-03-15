#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
//#include "AtlasStyle.C"

#define inpath1 "/home/atlas/Micromegas/ProdData/mapping/"
#define inpath2 "/home/atlas/Micromegas/ProdData/reference/"
#define outpath "/home/atlas/Micromegas/ProdData/root_plot/"

//macro fa diff run - reference
//vuole in ordine run diagonale sx e dx
//p=1,2 -> int,ext
void diff_panel_ref_diag (int p, string scan1, string scan2){

  // style option
   gROOT->LoadMacro("AtlasUtils.C");   // myText
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".3f");
   //gStyle->SetOptTitle(0);
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


// file containing the data
   ostringstream file1;
   file1 << inpath1  << scan1;
   scan1.erase(scan1.end()-4,scan1.end());
   scan1.erase(scan1.begin(),scan1.begin()+13);

   ostringstream file2;
   file2 << inpath1  << scan2;
   scan2.erase(scan2.end()-4,scan2.end());
   scan2.erase(scan2.begin(),scan2.begin()+13);

// reference files
   ostringstream file3;
   ostringstream file4; 

   if(p==1){

      file3 << inpath2<< "mean_and_rep_diag_laser_int_sx_ref_T_n.root";    
      file4 << inpath2<< "mean_and_rep_diag_laser_int_dx_ref_T_n.root";

   }
   else if (p==2){

      file3 << inpath2<< "mean_and_rep_diag_laser_ext_sx_ref_T_n.root";    
      file4 << inpath2<< "mean_and_rep_diag_laser_ext_dx_ref_T_n.root";

   }

   TFile *f1 = new TFile(file3.str().c_str());
   TH2F *ref_s1 = (TH2F*)f1->Get("mean_map");
   TH2F *ave_s1 = (TH2F*)ref_s1->Clone("average");
   ave_s1->SetTitle("Average Map S1");
   ave_s1->Scale(0.001);

   TFile *f2 = new TFile(file4.str().c_str());
   TH2F *ref_s2 = (TH2F*)f2->Get("mean_map");
   TH2F *ave_s2 = (TH2F*)ref_s2->Clone("average");
   ave_s2->SetTitle("Average Map S2");
   ave_s2->Scale(0.001);




   int nxbin=24;   // 
   int nybin=30;   //
   float xmin_s, xmax_s, ymin_s, ymax_s;
   float xmin_d, xmax_d, ymin_d, ymax_d;
   if (p==1) {
        xmin_s = 300/10;    //int sx (riferimento da pc)
        xmax_s = 2450/10;
        ymax_s = 550;
        ymin_s = 150;
  
        xmin_d = 320/10;   //int dx
        xmax_d = 2450/10;
        ymax_d = 1280;
        ymin_d = 870;
   }
   else if (p==2) {
        xmin_s = 330/10;   //int sx
        xmax_s = 2390/10;
        ymax_s = 465;
        ymin_s = 78;
   
        xmin_d = 330/10; //int dx
        xmax_d = 2390/10;
        ymax_d = 1360;
        ymin_d = 960;
   }

   
   TH2F *map_s1 = new TH2F("map_s1","map_s1", nxbin, xmin_s, xmax_s, nybin, ymin_s, ymax_s );
   map_s1->GetXaxis()->SetTitle("X (cm)");
   map_s1->GetYaxis()->SetTitle("Y (mm)");
   map_s1->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *map_s2 = new TH2F("map_s2","map_s2", nxbin, xmin_d, xmax_d, nybin, ymin_d, ymax_d );
   map_s2->GetXaxis()->SetTitle("X (cm)");
   map_s2->GetYaxis()->SetTitle("Y (mm)");
   map_s2->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *map_s1_p = new TH2F("map_s1_p","map_s1_p", nxbin, xmin_s, xmax_s, nybin, ymin_s, ymax_s );
   map_s1_p->GetXaxis()->SetTitle("X (cm)");
   map_s1_p->GetYaxis()->SetTitle("Y (mm)");
   map_s1_p->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *map_s2_p = new TH2F("map_s2_p","map_s2_p", nxbin, xmin_d, xmax_d, nybin, ymin_d, ymax_d );
   map_s2_p->GetXaxis()->SetTitle("X (cm)");
   map_s2_p->GetYaxis()->SetTitle("Y (mm)");
   map_s2_p->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *diff1 = new TH2F("diff1","diff1", nxbin, xmin_s, xmax_s, nybin, ymin_s, ymax_s );
   diff1->GetXaxis()->SetTitle("X (cm)");
   diff1->GetYaxis()->SetTitle("Y (mm)");
   diff1->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *diff2 = new TH2F("diff2","diff2", nxbin, xmin_s, xmax_s, nybin, ymin_s, ymax_s );
   diff2->GetXaxis()->SetTitle("X (cm)");
   diff2->GetYaxis()->SetTitle("Y (mm)");
   diff2->GetZaxis()->SetTitle("Z (#mu m)");

 
   int ndistbin = 50; //cambiare se sono PCB o pannello
   float xdistmin =11.300; 
   float xdistmax =11.700; 


   TH1F *distz_s1 = new TH1F("distz_s1","distz_s1", ndistbin,xdistmin, xdistmax  ); 
   distz_s1->GetXaxis()->SetTitle("#mu ");
   distz_s1->GetYaxis()->SetTitle("counts");
   distz_s1->SetFillColor(2);

   TH1F *distz_s2 = new TH1F("distz_s2","distz_s2",  ndistbin,xdistmin, xdistmax  ); 
   distz_s2->GetXaxis()->SetTitle("#mu ");
   distz_s2->GetYaxis()->SetTitle("counts");
   distz_s2->SetFillColor(2);

   TH1F *distz_diff = new TH1F("distz_diff","distz_diff",  ndistbin,xdistmin, xdistmax  );
   distz_diff->GetXaxis()->SetTitle("#mu ");
   distz_diff->GetYaxis()->SetTitle("counts");
   distz_diff->SetFillColor(2);
      
   Int_t j=0; 
   Float_t x,y, opt, laser, temp1, temp2, coord;
   Double_t media,stdev,minimo,massimo;



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
	   //distz_s1->Fill(coord);
           map_s1->Fill(x/10,-y, coord);
           map_s1_p->Fill(x/10,-y, coord-11.80); // 11.80 spessore pannello
	   j++;
    }

    j=0; 
    ifstream in2(file2.str().c_str());
    if (!in2 || in2.bad()) return;

    while(1){
      
           in2 >> x >> y >> opt >> laser >>  temp1 >> temp2;
	   if(!in2.good()) break; 

	   coord = (laser - opt);
           //scan[1][j]=coord*1000;
	   //m2 += scan[1][j];
	   cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " << temp2 << "c2=  " << coord << " " <<  endl;
	   //distz_s2->Fill(coord);
           map_s2->Fill(x/10,-y, coord);
           map_s2_p->Fill(x/10,-y, coord-11.80);
	   j++;
	   
    }
    
    diff1->Add(ave_s1,map_s1,-1,+1);
    diff2->Add(ave_s2,map_s2,-1,+1);


    float zref1,zmap1, z1, zref2,zmap2,z2; 
    
   for (int i=1; i<=nybin; i++){
      for (int j=1; j<=nxbin; j++){
	  zref1 = ave_s1->GetBinContent(j,i);
	  zmap1 = map_s1->GetBinContent(j,i);
	  z1 = diff1->GetBinContent(j,i);
	  if(z1!=0) {
	     distz_s1->Fill(z1);
	     distz_diff->Fill(z1);
	     cout << zref1 << " " << zmap1 << " z1=  " << z1 << endl;
          }
	  zref2 = ave_s2->GetBinContent(j,i);
	  zmap2 = map_s2->GetBinContent(j,i);
	  z2 = diff2->GetBinContent(j,i);
	  if(z2!=0) {
             distz_s2->Fill(z2); 
	     distz_diff->Fill(z2);
	     cout << zref2 << " " << zmap2 << " z2=  " << z2 << endl;

         }
      }
    }

   media =  distz_diff->GetMean();
   stdev =  distz_diff->GetRMS();
   minimo =   distz_diff->GetMinimum();
   massimo =   distz_diff->GetMaximum();

   cout << " media= " << media << " rms= " << stdev << " min= " << minimo << "max= " << massimo << endl;


 // create a root file for the histograms;
  ostringstream rootfile;
  rootfile  << outpath <<"/diff_pan_plate_diag_" << scan1 << "_" << scan2 << ".root";   //*************!!!!!!
  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;

  //create a txt data file;
  ofstream Data;
  ostringstream txtfile;
  txtfile << outpath <<"/summary_diff_pan_plate_diag_" << scan1 << ".txt"; 
  Data.open(txtfile.str().c_str(), ofstream::out | ofstream::app);
  cout << "Text output file: " << endl << txtfile.str() << endl;
  Data << "Mean   rms   min   max" << "\n" << media << "\t" << stdev << "\t" << minimo << "\t" << massimo << endl;

  ostringstream mapcorr1;
  mapcorr1 << outpath << "/diff_pan_plate_diag_mapSX_" << scan1 << "_" << scan2 <<".png";

  ostringstream mapcorr2;
  mapcorr2 << outpath << "/diff_pan_plate_diag_mapDX_" << scan1 << "_" << scan2 << ".png";

  ostringstream zcorr;
  zcorr << outpath << "/diff_pan_plate_diag_zdistrib_" << scan1 << "_" << scan2 << ".png";


   TCanvas *c0 = new TCanvas("map_sx","map_sx",1000,700);
   map_s1->SetMarkerSize(0.8);
   map_s1->Draw("colz1,text45");
   map_s1->SetStats(kFALSE);
   map_s1->GetZaxis()->SetRangeUser(11.3, 11.70);
   map_s1->Write();

   TCanvas *c1 = new TCanvas("map_dx","map_dx",1000,700);
   map_s2->SetMarkerSize(0.8);
   map_s2->Draw("colz1,text45");
   map_s2->SetStats(kFALSE);
   map_s2->GetZaxis()->SetRangeUser(11.3, 11.70);
   map_s2->Write();

   TCanvas *c2 = new TCanvas("ref_sx","ref_sx",1000,700);
   ref_s1->SetMarkerSize(0.8);
   ref_s1->Draw("colz1,text45");
   ref_s1->SetStats(kFALSE);
   ref_s1->Write();

   TCanvas *c3 = new TCanvas("ref_dx","ref_dx",1000,700);
   ref_s2->SetMarkerSize(0.8);
   ref_s2->Draw("colz1,text45");
   ref_s2->SetStats(kFALSE);
   ref_s2->Write();



   TCanvas *c4 = new TCanvas("diff_sx","diff_sx",1000,700);
   diff1->SetMarkerSize(0.8);
   diff1->Draw("colz1,text45");
   diff1->SetStats(kFALSE);
   diff1->GetZaxis()->SetRangeUser(11.3, 11.70);
   diff1->Write();
   c4->Write(mapcorr1.str().c_str());

   TCanvas *c5 = new TCanvas("diff_dx","diff_dx",1000,700);
   diff2->SetMarkerSize(0.8);
   diff2->Draw("colz1,text45");
   diff2->SetStats(kFALSE);
   diff2->GetZaxis()->SetRangeUser(11.3, 11.70);
   diff2->Write();
   c5->Write(mapcorr2.str().c_str());

   TCanvas *c21 = new TCanvas("c21","c21",1000,700);
   distz_s1->Draw();
   distz_s1->Write();

   TCanvas *c22 = new TCanvas("c22","c22",1000,700);
   distz_s2->Draw();
   distz_s2->Write();

   TCanvas *c23 = new TCanvas("c23","c23",1000,700);
   distz_diff->Draw();
   distz_diff->Write();   
   c23->Write(zcorr.str().c_str());   


  myroot.Close();

   return; 
}





















