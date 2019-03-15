#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
//#include "AtlasStyle.C"

#define inpath "/home/atlas/Micromegas/ProdData/mapping/"
#define outpath "/home/atlas/Micromegas/ProdData/reference/"


int count_lines(string scan1)

{
   int count = 0;
   string line;
 
    /* Creating input filestream */ 
    ifstream file(scan1);
    while (getline(file, line))
    count++;
 
    cout << "Numbers of lines in the file : " << count << endl;
    return count;
}


//p=0,1,2 -> reference,pannello,pcb

void repeatability_and_mean (int p, string scan1, string scan2, string scan3){


  // style option
   gROOT->LoadMacro("AtlasUtils.C");   // myText
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".0f");
   //gStyle->SetOptTitle(0);
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


// file containing the data
   ostringstream file1;
   file1 << inpath  << scan1;
   scan1.erase(scan1.end()-4,scan1.end());
   scan1.erase(scan1.begin(),scan1.begin()+13);

   ostringstream file2;
   file2 << inpath  << scan2;
   scan2.erase(scan2.end()-4,scan2.end());
   scan2.erase(scan2.begin(),scan2.begin()+13);

   ostringstream file3;
   file3 << inpath  << scan3;
   scan3.erase(scan3.end()-4,scan3.end());
   scan3.erase(scan3.begin(),scan3.begin()+13);

  string nomefile=file1.str();
 
   int nxbin=24;   // 
   int nybin=30;   //
   float xmin = 320/10;
   float xmax = 2440/10;
   float ymax = 1230;
   float ymin = 200;
   
   TH2F *map_s1 = new TH2F("map_s1","map_s1", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s1->GetXaxis()->SetTitle("X (cm)");
   map_s1->GetYaxis()->SetTitle("Y (mm)");
   map_s1->GetZaxis()->SetTitle("Z (#mu m)");


   TH2F *map_s2 = new TH2F("map_s2","map_s2", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s2->GetXaxis()->SetTitle("X (cm)");
   map_s2->GetYaxis()->SetTitle("Y (mm)");
   map_s2->GetZaxis()->SetTitle("Z (#mu m)");


   TH2F *map_s3 = new TH2F("map_s3","map_s3", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s3->GetXaxis()->SetTitle("X (cm)");
   map_s3->GetYaxis()->SetTitle("Y (mm)");
   map_s3->GetZaxis()->SetTitle("Z (#mu m)");


   float xdistmin, xdistmax;
   int ndistbin = 100;
   if(p==0){              //piastra reference
   xdistmin = -150; 
   xdistmax = 150; 
   }
   if (p==1){             //pannello 
   xdistmin = 11000; 
   xdistmax = 11900;
   }
   if (p==2){
   xdistmin = 650;        //z_pcb=770um
   xdistmax = 850;
   }


   TH1F *distz_s1 = new TH1F("distz_s1","distz_s1", ndistbin,xdistmin, xdistmax  ); 
   distz_s1->GetXaxis()->SetTitle("#mu ");
   distz_s1->GetYaxis()->SetTitle("counts");
   distz_s1->SetFillColor(2);

   TH1F *distz_s2 = new TH1F("distz_s2","distz_s2",  ndistbin,xdistmin, xdistmax  ); 
   distz_s2->GetXaxis()->SetTitle("#mu ");
   distz_s2->GetYaxis()->SetTitle("counts");
   distz_s2->SetFillColor(2);

   TH1F *distz_s3 = new TH1F("distz_s3","distz_s3",  ndistbin,xdistmin, xdistmax  ); 
   distz_s3->GetXaxis()->SetTitle("#mu ");
   distz_s3->GetYaxis()->SetTitle("counts");
   distz_s3->SetFillColor(2);
      
   
   TH2F *mean_map = new TH2F("mean_map", "average Map", nxbin, xmin, xmax, nybin, ymin, ymax );
   mean_map->GetXaxis()->SetTitle("X (mm)");
   mean_map->GetYaxis()->SetTitle("Y (mm)");
   mean_map->GetZaxis()->SetTitle("Z (#mum)");


   TH1F *mean_distrib = new TH1F("Average_distrib","Average Distribution", ndistbin,xdistmin, xdistmax );
   mean_distrib->GetXaxis()->SetTitle("Z (#mum)");
   mean_distrib->SetFillColor(3);

   TH2F *hzmin= new TH2F("hzmin","hzmin", nxbin, xmin, xmax, nybin, ymin, ymax );
   hzmin->GetXaxis()->SetTitle("X (mm)");
   hzmin->GetYaxis()->SetTitle("Y (mm)");
   hzmin->GetZaxis()->SetTitle("Z (#mum)");

   TH2F *hzmax= new TH2F("hzmax","hzmax", nxbin, xmin, xmax, nybin, ymin, ymax );
   hzmax->GetXaxis()->SetTitle("X (mm)");
   hzmax->GetYaxis()->SetTitle("Y (mm)");
   hzmax->GetZaxis()->SetTitle("Z (#mum)");

   TH2F *hdz= new TH2F("Repeatability Map","Repeatability Map", nxbin, xmin, xmax, nybin, ymin, ymax );
   hdz->GetXaxis()->SetTitle("X (mm)");
   hdz->GetYaxis()->SetTitle("Y (mm)");
   hdz->GetZaxis()->SetTitle("Z (#mum)");

   TH1F *dz_distrib = new TH1F("dz_distrib","Repeatability Distribution", 50, -5, 95);
   dz_distrib->GetXaxis()->SetTitle("Z (#mum)");
   dz_distrib->SetFillColor(2);

   TH2F *std_map= new TH2F("std_map","std_map", nxbin, xmin, xmax, nybin, ymin, ymax );
   std_map->GetXaxis()->SetTitle("X (mm)");
   std_map->GetYaxis()->SetTitle("Y (mm)");
   std_map->GetZaxis()->SetTitle("Z (#mum)");

   TH1F *std_distrib = new TH1F("std_distrib","std Distribution", 30, -2, 28);
   std_distrib->GetXaxis()->SetTitle("Z (#mum)");
   std_distrib->SetFillColor(4);

   Float_t dd1, dd2, dd3, mean; 
   Float_t m1, m2, m3; 
   Float_t d1, d2, d3, dd, mean2, max, min, dz;
   Float_t dz1_mean, dz2_mean, dz3_mean, std;
   
   int n;
   n=count_lines(nomefile);
   cout<<"n="<<n<<"scan="<<nomefile<<endl;

   Float_t scan[7][266]={0};
   Float_t x,y, opt, laser, temp1, temp2, coord;
   Double_t media,stdev,minimo,massimo;

// leggo file e riempio array con coord

    ifstream in1(file1.str().c_str());
    for (int j=0; j<n; j++){
           in1 >> x >> y >> opt >> laser >>  temp1 >> temp2;
           coord = (laser - opt);
           scan[0][j]=coord*1000;
	   m1 += scan[0][j];
	   cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " << temp2 << " " << endl;
    }
    m1=m1/n; 
    //    cout << " m1 " << m1 << endl; 
    cout << "file2" << endl;

    ifstream in2(file2.str().c_str());
    for (int j=0; j<n; j++){
           in2 >> x >> y >> opt >> laser >> temp1 >> temp2;
           coord = (laser - opt);
            scan[1][j]=coord*1000;
	    m2 += scan[1][j];
	   cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " << temp2 << " " << endl;

    }
    m2=m2/n; 
    //    cout << " m2 " << m2 << endl; 
    cout << "file3" << endl;

    ifstream in3(file3.str().c_str());
    for (int j=0; j<n; j++){
           in3 >> x >> y >> opt >> laser  >> temp1 >> temp2;
           coord = (laser - opt);
           scan[2][j]=coord*1000;
	   m3 += scan[2][j];
	   cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " << temp2 << " " << endl;

    }
    m3=m3/n; 
    //    cout << " m3 " << m3 << endl; 

// leggo file e riempio array con coord x e y
  ifstream in6 (file1.str().c_str());
  
     for (int j=0; j<n; j++){
             in6 >> x >> y >> opt >> laser >>  temp1 >> temp2; // !!!!!!!!!!!!
             scan[5][j]=x/10;
             scan[6][j]=-y;
             //cout << "x= " << scan[5][j] << "y=  " << scan[6][j] << endl;
   } 
/*
// tolgo i buchi
   for (int i=0; i<=4; i++){
      for (int j=0; j<=n; j++){
          if(scan[i][j] < -10 || scan[i][j] > 120 ) {
	    //	     cout << " scan "<< i << " point " << j << " value " << scan[i][j] << endl;
	     if(scan[i][j+1] < -10 ||scan[i][j+1] > 120 || scan[i][j+1] ==0){
		if(scan[i][j-1] < -10 ||scan[i][j-1] > 120 || scan[i][j-1] ==0) 
			scan[i][j]=(scan[i][j+2]+scan[i][j-2])/2;
		else scan[i][j]=scan[i][j-1];
	     }
	     if(scan[i][j-1] < -10 ||scan[i][j-1] > 120 || scan[i][j-1] ==0){
		if(scan[i][j+1] < -10 ||scan[i][j+1] > 120 || scan[i][j+1] ==0) 
			scan[i][j]=(scan[i][j+2]+scan[i][j-2])/2;
		else scan[i][j]=scan[i][j+1];
	     }      
	     else scan[i][j]=(scan[i][j-1]+scan[i][j+1])/2;
	     //   cout << "scan[i][j-1] " << scan[i][j-1] << " scan[i][j+1] " << scan[i][j+1] 
	     //		 << " new value " << scan[i][j] << endl; 
	  }
      }
   }
*/

// leggo array e calcolo media e std di ogni punto
   for (int j=0; j<n; j++){
      d1 = scan[0][j];
      d2 = scan[1][j];
      d3 = scan[2][j];
      //      cout << d1 << " " << d2 << " " << d3 << endl; 
      mean2 = (d1+d2+d3)/3;
      //     cout << "mean " << mean2 << endl;

      map_s1->Fill(scan[5][j],scan[6][j], d1);
      map_s2->Fill(scan[5][j],scan[6][j], d2);
      map_s3->Fill(scan[5][j],scan[6][j], d3);
      distz_s1->Fill(d1);
      distz_s2->Fill(d2);
      distz_s3->Fill(d3);

      mean_map->Fill(scan[5][j],scan[6][j], mean2);
      mean_distrib->Fill(mean2);

      dz1_mean = (scan[0][j] - mean2);
      dz2_mean = (scan[1][j] - mean2);
      dz3_mean = (scan[2][j] - mean2);

      std = sqrt((dz1_mean*dz1_mean + dz2_mean*dz2_mean + dz3_mean*dz3_mean)/2);
      std_distrib->Fill(std);
      std_map->Fill(scan[5][j],scan[6][j],std);
   }

   media = mean_distrib->GetMean();
   stdev = mean_distrib->GetRMS();
   minimo =  mean_distrib->GetMinimum();
   massimo =  mean_distrib->GetMaximum();

   cout << " media= " << media << " stdev= " << stdev << " min= " << minimo << " max= " << massimo << endl;

// leggo array e calcolo rep come dz

   for (int j=0; j<n; j++){
//      max=-1000;
//      min=1000;
      max=-12000;
      min=12000;

      for (int i=0; i<=2; i++){
          if(scan[i][j]<min){min = scan[i][j];}
          if(scan[i][j]>max){max = scan[i][j];}
      }
    dz=max-min;
    dz_distrib->Fill(dz);
    //    cout<<"dz="<<dz<<endl;
    hdz->Fill(scan[5][j],scan[6][j],dz);
   }


 // create a root file for the histograms;
  ostringstream rootfile;
  
  if(p==0){
    //    rootfile  << outpath <<"mean_and_rep_plates_laser_ref_tab.root";
    rootfile  << outpath <<"mean_and_rep_plates_laser_ref_stif_new.root";
    //rootfile  << outpath <<"mean_and_rep_plates_laser_ref_tab_new.root";

  }
  if(p==1){
           rootfile  << outpath <<"mean_and_rep_plates_panel.root";
  }
  

  //create a txt data file;
  ofstream Data;
  ostringstream txtfile;
  txtfile << outpath <<"/summary_" << scan1 << ".txt"; 



  TFile myroot(rootfile.str().c_str(), "RECREATE");
  cout << "Root output file: " << endl << rootfile.str() << endl;

  ostringstream Mean_map;
  Mean_map << outpath << "/mean_map.png";

  ostringstream Mean_distrib;
  Mean_distrib << outpath << "/mean_distrib.png";

  ostringstream Dz_map;
  Dz_map << outpath << "/dz_map.png";

  ostringstream Dz_distrib;
  Dz_distrib << outpath << "/dz_distrib.png";

  ostringstream Std_map;
  Std_map << outpath << "/std_map.png";

  ostringstream Std_distrib;
  Std_distrib << outpath << "/std_distrib.png";


   TCanvas *c0 = new TCanvas("c0","c0",1000,700);
   map_s1->SetMarkerSize(0.8);
   map_s1->Draw("colz1,text45");
   map_s1->SetStats(kFALSE);
   map_s1->Write();

   TCanvas *c1 = new TCanvas("c1","c1",1000,700);
   map_s2->SetMarkerSize(0.8);
   map_s2->Draw("colz1,text45");
   map_s2->SetStats(kFALSE);
   map_s2->Write();

   TCanvas *c2 = new TCanvas("c2","c2",1000,700);
   map_s3->SetMarkerSize(0.8);
   map_s3->Draw("colz1,text45");
   map_s3->SetStats(kFALSE);
   map_s3->Write();


   TCanvas *c21 = new TCanvas("c21","c21",1000,700);
   distz_s1->Draw();
   distz_s1->Write();

   TCanvas *c22 = new TCanvas("c22","c22",1000,700);
   distz_s2->Draw();
   distz_s2->Write();

   TCanvas *c23 = new TCanvas("c23","c23",1000,700);
   distz_s3->Draw();
   distz_s3->Write();
   

   TCanvas *cc2 = new TCanvas("cc2","cc2",1000,700);  
   mean_map->SetMarkerSize(0.8);
   mean_map->SetStats(kFALSE);
   mean_map->Draw("colz1,text45"); 
   mean_map->Write();
   cc2->Print(Mean_map.str().c_str());

   TCanvas *cc3 = new TCanvas("cc3","cc3",1000,700); 
   mean_distrib->Draw(); 
   mean_distrib->Write();
   cc3->Print(Mean_distrib.str().c_str());

   TCanvas *c9 = new TCanvas("c9","c9",1000,700); 
   hdz->SetMarkerSize(0.8); 
   hdz->SetStats(kFALSE);
   hdz->Draw("colz1,text0"); 
   hdz->Write();
   c9->Print(Dz_map.str().c_str());

   TCanvas *c6 = new TCanvas("c6","c6",1000,700); 
   dz_distrib->Draw(); 
   dz_distrib->Write();
   c6->Print(Dz_distrib.str().c_str());

   TCanvas *c16 = new TCanvas("c16","c16",1000,700);  
   std_map->SetMarkerSize(0.8);
   std_map->SetStats(kFALSE);
   std_map->Draw("colz1,text0"); 
   std_map->Write();
   c16->Print(Std_map.str().c_str());

   TCanvas *c26 = new TCanvas("c26","c26",1000,700);  
   std_distrib->Draw(); 
   std_distrib->Write();
   c26->Print(Std_distrib.str().c_str());
   
   myroot.Close();

   return; 
}





















