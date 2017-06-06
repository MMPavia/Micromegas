#include <stdio.h>
#include <string>
#include <vector>
#include <math.h>
//#include "AtlasStyle.C"


//#define inpath1 "/home/atlas/Micromegas/M05Data/mapping/"
//#define inpath "/home/atlas/Micromegas/M05Data/root_plot/"
//#define outpath "/home/atlas/Micromegas/M05Data/root_plot/"
#define inpath "/Users/mariacristinalopresti/Desktop/mm/gauge-laser/pcb_zone/"
#define outpath "/Users/mariacristinalopresti/Desktop/mm/gauge-laser/pcb_zone/"


int count_lines(string scan)

{
   int count = 0;
   string line;
 
    /* Creating input filestream */ 
    ifstream file(scan);
    while (getline(file, line))
    count++;
 
    cout << "Numbers of lines in the file : " << count << endl;
    return count;
}




//q=0,1 ->laser,gauge

void prova_pcb (int q, string scan){

  // style option
  // gROOT->LoadMacro("AtlasUtils.C");   // myText
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".3f");
   //gStyle->SetOptTitle(0);
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


// file containing the data.txt
   ostringstream file1;
   file1 << inpath  << scan;
   scan.erase(scan.end()-4,scan.end());
   scan.erase(scan.begin(),scan.begin()+13);

  string nomefile=file1.str();


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
   TH2F *ref_s5 = (TH2F*)f1->Get("mean_map5");
   TH2F *ave_s5 = (TH2F*)ref_s5->Clone("average5");
   ave_s5->SetTitle("Average Reference Map5");
   ave_s5->Scale(0.001);
   TH2F *ref_s4 = (TH2F*)f1->Get("mean_map4");
   TH2F *ave_s4 = (TH2F*)ref_s4->Clone("average4");
   ave_s4->SetTitle("Average Reference Map4");
   ave_s4->Scale(0.001);
   TH2F *ref_s3 = (TH2F*)f1->Get("mean_map3");
   TH2F *ave_s3 = (TH2F*)ref_s3->Clone("average3");
   ave_s3->SetTitle("Average Reference Map3");
   ave_s3->Scale(0.001);
   TH2F *ref_s2 = (TH2F*)f1->Get("mean_map2");
   TH2F *ave_s2 = (TH2F*)ref_s2->Clone("average2");
   ave_s2->SetTitle("Average Reference Map2");
   ave_s2->Scale(0.001);
   TH2F *ref_s1 = (TH2F*)f1->Get("mean_map1");
   TH2F *ave_s1 = (TH2F*)ref_s1->Clone("average1");
   ave_s1->SetTitle("Average Reference Map1");
   ave_s1->Scale(0.001);



   int nxbin=24;   // 
   int nybin=30;   //
   float xmin = 253/10;
   float xmax = 2414/10;
   float ymax = 1239;
   float ymin = 162;
   float xmax5= 686/10; 
    
    
   TH2F *map_s = new TH2F("map_s","map_s", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s->GetXaxis()->SetTitle("X (cm)");
   map_s->GetYaxis()->SetTitle("Y (mm)");
   map_s->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *map_s5 = new TH2F("map_s5","map_s5", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s5->GetXaxis()->SetTitle("X (cm)");
   map_s5->GetYaxis()->SetTitle("Y (mm)");
   map_s5->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *map_s4 = new TH2F("map_s4","map_s4", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s4->GetXaxis()->SetTitle("X (cm)");
   map_s4->GetYaxis()->SetTitle("Y (mm)");
   map_s4->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *map_s3 = new TH2F("map_s3","map_s3", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s3->GetXaxis()->SetTitle("X (cm)");
   map_s3->GetYaxis()->SetTitle("Y (mm)");
   map_s3->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *map_s2 = new TH2F("map_s2","map_s2", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s2->GetXaxis()->SetTitle("X (cm)");
   map_s2->GetYaxis()->SetTitle("Y (mm)");
   map_s2->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *map_s1 = new TH2F("map_s1","map_s1", nxbin, xmin, xmax, nybin, ymin, ymax );
   map_s1->GetXaxis()->SetTitle("X (cm)");
   map_s1->GetYaxis()->SetTitle("Y (mm)");
   map_s1->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *diff = new TH2F("diff","diff", nxbin, xmin, xmax, nybin, ymin, ymax );
   diff->GetXaxis()->SetTitle("X (cm)");
   diff->GetYaxis()->SetTitle("Y (mm)");
   diff->GetZaxis()->SetTitle("Z (#mu m)");

   TH2F *diff5 = new TH2F("diff5","diff5", nxbin, xmin, xmax, nybin, ymin, ymax ); //faccio le 5 mappe dei pcb
   diff5->GetXaxis()->SetTitle("X (cm)");
   diff5->GetYaxis()->SetTitle("Y (mm)");
   diff5->GetZaxis()->SetTitle("Z (#mu m)");
   
   TH2F *diff4 = new TH2F("diff4","diff4", nxbin, xmin, xmax, nybin, ymin, ymax );
   diff4->GetXaxis()->SetTitle("X (cm)");
   diff4->GetYaxis()->SetTitle("Y (mm)");
   diff4->GetZaxis()->SetTitle("Z (#mu m)");
   
   TH2F *diff3 = new TH2F("diff3","diff3", nxbin, xmin, xmax, nybin, ymin, ymax );
   diff3->GetXaxis()->SetTitle("X (cm)");
   diff3->GetYaxis()->SetTitle("Y (mm)");
   diff3->GetZaxis()->SetTitle("Z (#mu m)");
   
   TH2F *diff2 = new TH2F("diff2","diff2", nxbin, xmin, xmax, nybin, ymin, ymax );
   diff2->GetXaxis()->SetTitle("X (cm)");
   diff2->GetYaxis()->SetTitle("Y (mm)");
   diff2->GetZaxis()->SetTitle("Z (#mu m)");
   
   TH2F *diff1 = new TH2F("diff1","diff1", nxbin, xmin, xmax, nybin, ymin, ymax ); //fine 5 mappe pcb
   diff1->GetXaxis()->SetTitle("X (cm)");
   diff1->GetYaxis()->SetTitle("Y (mm)");
   diff1->GetZaxis()->SetTitle("Z (#mu m)");
   
   

   int ndistbin = 100; 
   float xdistmin =0.650; 
   float xdistmax =0.850; 

   TH1F *distz_s = new TH1F("distz_s","distz_s", ndistbin,xdistmin, xdistmax  ); 
   distz_s->GetXaxis()->SetTitle("#mu ");
   distz_s->GetYaxis()->SetTitle("counts");
   distz_s->SetFillColor(2);

   TH1F *distz_diff = new TH1F("distz_diff","distz_diff",  ndistbin,xdistmin, xdistmax  );
   distz_diff->GetXaxis()->SetTitle("#mu ");
   distz_diff->GetYaxis()->SetTitle("counts");
   distz_diff->SetFillColor(2);
   
   TH1F *distz_diff5 = new TH1F("distz_diff5","distz_diff5",  ndistbin,xdistmin, xdistmax  );  //faccio i 5 isto pcb
   distz_diff5->GetXaxis()->SetTitle("#mu ");
   distz_diff5->GetYaxis()->SetTitle("counts");
   distz_diff5->SetFillColor(2);   

   TH1F *distz_diff4 = new TH1F("distz_diff4","distz_diff4",  ndistbin,xdistmin, xdistmax  );
   distz_diff4->GetXaxis()->SetTitle("#mu ");
   distz_diff4->GetYaxis()->SetTitle("counts");
   distz_diff4->SetFillColor(2);   

   TH1F *distz_diff3 = new TH1F("distz_diff3","distz_diff3",  ndistbin,xdistmin, xdistmax  );
   distz_diff3->GetXaxis()->SetTitle("#mu ");
   distz_diff3->GetYaxis()->SetTitle("counts");
   distz_diff3->SetFillColor(2);

   TH1F *distz_diff2 = new TH1F("distz_diff2","distz_diff2",  ndistbin,xdistmin, xdistmax  );
   distz_diff2->GetXaxis()->SetTitle("#mu ");
   distz_diff2->GetYaxis()->SetTitle("counts");
   distz_diff2->SetFillColor(2);
   Int_t j=0; 
   
   TH1F *distz_diff1 = new TH1F("distz_diff1","distz_diff1",  ndistbin,xdistmin, xdistmax  ); //fine 5 isto pcb
   distz_diff1->GetXaxis()->SetTitle("#mu ");
   distz_diff1->GetYaxis()->SetTitle("counts");
   distz_diff1->SetFillColor(2);
   
   
   
      

   Float_t x,y, opt, laser, temp1, temp2, coord, zprova;

   int n;
   n=count_lines(nomefile);
   cout<<"n="<<n<<"scan="<<nomefile<<endl;



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

    ifstream in2(file1.str().c_str());
    if (!in2 || in2.bad()) return;

      for (int j=0; j<64; j++){                      //pcb5
      
       in2 >> x >> y >> opt >> laser >>  temp1 >> temp2;
	   if(!in2.good()) break; 

	   coord = (laser - opt);
	   //scan[0][j]=coord*1000;
	   //m1 += scan[0][j];
	   //cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " << temp2 << "c1=  " << coord << " "  << endl;
	   //distz_s5->Fill(coord);
	   
	   if(q==1){
           map_s5->Fill((x+40.0)/10,-y, coord);
	       }
	   if(q==0){ 
           map_s5->Fill(x/10,-y, coord);
           }
        
	   j++;
    }

    diff5->Add(ave_s5,map_s5,-1,+1);

    ifstream in3(file1.str().c_str());
    if (!in3 || in3.bad()) return;

      for (int j=65; j<119; j++){                     //pcb4
      
       in3 >> x >> y >> opt >> laser >>  temp1 >> temp2;
	   if(!in3.good()) break; 

	   coord = (laser - opt);
	   //scan[0][j]=coord*1000;
	   //m1 += scan[0][j];
	   //cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " << temp2 << "c1=  " << coord << " "  << endl;
	   //distz_s5->Fill(coord);
	   
	   if(q==1){
           map_s4->Fill((x+40.0)/10,-y, coord);
	       }
	   if(q==0){ 
           map_s4->Fill(x/10,-y, coord);
           }
        
	   j++;
    }

    diff4->Add(ave_s4,map_s4,-1,+1);

    ifstream in4(file1.str().c_str());
    if (!in4 || in4.bad()) return;

      for (int j=120; j<156; j++){                     //pcb3
      
       in4 >> x >> y >> opt >> laser >>  temp1 >> temp2;
	   if(!in4.good()) break; 

	   coord = (laser - opt);
	   //scan[0][j]=coord*1000;
	   //m1 += scan[0][j];
	   //cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " << temp2 << "c1=  " << coord << " "  << endl;
	   //distz_s5->Fill(coord);
	   
	   if(q==1){
           map_s3->Fill((x+40.0)/10,-y, coord);
	       }
	   if(q==0){ 
           map_s3->Fill(x/10,-y, coord);
           }
        
	   j++;
    }

    diff3->Add(ave_s3,map_s3,-1,+1);

    ifstream in5(file1.str().c_str());
    if (!in5 || in5.bad()) return;

      for (int j=157; j<194; j++){                     //pcb2
      
       in5 >> x >> y >> opt >> laser >>  temp1 >> temp2;
	   if(!in5.good()) break; 

	   coord = (laser - opt);
	   //scan[0][j]=coord*1000;
	   //m1 += scan[0][j];
	   //cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " << temp2 << "c1=  " << coord << " "  << endl;
	   //distz_s5->Fill(coord);
	   
	   if(q==1){
           map_s2->Fill((x+40.0)/10,-y, coord);
	       }
	   if(q==0){ 
           map_s2->Fill(x/10,-y, coord);
           }
        
	   j++;
    }

    diff2->Add(ave_s2,map_s2,-1,+1);

    ifstream in6(file1.str().c_str());
    if (!in6 || in6.bad()) return;

      for (int j=195; j<n; j++){                     //pcb1
      
       in6 >> x >> y >> opt >> laser >>  temp1 >> temp2;
	   if(!in6.good()) break; 

	   coord = (laser - opt);
	   //scan[0][j]=coord*1000;
	   //m1 += scan[0][j];
	   //cout << x << " " << y << " " << opt << " "<< laser << " " <<  temp1 << " " << temp2 << "c1=  " << coord << " "  << endl;
	   //distz_s5->Fill(coord);
	   
	   if(q==1){
           map_s1->Fill((x+40.0)/10,-y, coord);
	       }
	   if(q==0){ 
           map_s1->Fill(x/10,-y, coord);
           }
        
	   j++;
    }

    diff1->Add(ave_s1,map_s1,-1,+1);


   float zref,zmap, z;
   float zref5, zmap5, z5, zfer4, zmap4, z4, zref3, zmap3, z3;
   float zref2, zmap2, z2, zref1, zmap1, z1; 
    
   for (int i=1; i<=nybin; i++){
      for (int j=1; j<=nxbin; j++){
	  zref = ave_s->GetBinContent(j,i);
	  zmap = map_s->GetBinContent(j,i);
	  z = diff->GetBinContent(j,i);
	  zref5 = ave_s5->GetBinContent(j,i);
	  zmap5 = map_s5->GetBinContent(j,i);
	  z5 = diff4->GetBinContent(j,i);
	  zref4 = ave_s4->GetBinContent(j,i);
	  zmap4 = map_s4->GetBinContent(j,i);
	  z4 = diff4->GetBinContent(j,i);
	  zref3 = ave_s3->GetBinContent(j,i);
	  zmap3 = map_s3->GetBinContent(j,i);
	  z3 = diff3->GetBinContent(j,i);
	  zref2 = ave_s2->GetBinContent(j,i);
	  zmap2 = map_s2->GetBinContent(j,i);
	  z2 = diff2->GetBinContent(j,i);
	  zref1 = ave_s1->GetBinContent(j,i);
	  zmap1 = map_s1->GetBinContent(j,i);
	  z1 = diff1->GetBinContent(j,i);
	  
	  if(z!=0) {
	     distz_diff->Fill(z);
	     distz_diff5->Fill(z5);
	     distz_diff4->Fill(z4);
	     distz_diff3->Fill(z3);
	     distz_diff2->Fill(z2);
	     distz_diff1->Fill(z1);

	     cout << zref << " " << zmap << " z=  " << z << endl;
          }
      }
    }
    cout << "ciao" << endl;



  Double_t media, RMS, media5, RMS5, media4, RMS4, media3, RMS3, media2, RMS2, media1, RMS1;

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

  // output data file 5
  ofstream Data5;
  ostringstream textfile5;
  textfile5 << outpath << "pcb_5_" << scan <<".dat"; 
  Data5.open( textfile5.str().c_str(), ofstream::out | ofstream::app);
  //TFile mytext(textfile.str().c_str(), "RECREATE");
  cout << "Text output file5: " << endl << textfile5.str() << endl;
  // output data file 4
  ofstream Data4;
  ostringstream textfile4;
  textfile4 << outpath << "pcb_4_" << scan <<".dat"; 
  Data4.open( textfile4.str().c_str(), ofstream::out | ofstream::app);
  //TFile mytext(textfile.str().c_str(), "RECREATE");
  cout << "Text output file4: " << endl << textfile4.str() << endl;
  // output data file 3
  ofstream Data3;
  ostringstream textfile3;
  textfile3 << outpath << "pcb_3_" << scan <<".dat"; 
  Data3.open( textfile3.str().c_str(), ofstream::out | ofstream::app);
  //TFile mytext(textfile.str().c_str(), "RECREATE");
  cout << "Text output file3: " << endl << textfile3.str() << endl;
    // output data file 2
  ofstream Data2;
  ostringstream textfile2;
  textfile2 << outpath << "pcb_2_" << scan <<".dat"; 
  Data2.open( textfile2.str().c_str(), ofstream::out | ofstream::app);
  //TFile mytext(textfile.str().c_str(), "RECREATE");
  cout << "Text output file2: " << endl << textfile2.str() << endl;
  // output data file 1
  ofstream Data1;
  ostringstream textfile1;
  textfile1 << outpath << "pcb_1_" << scan <<".dat"; 
  Data1.open( textfile1.str().c_str(), ofstream::out | ofstream::app);
  //TFile mytext(textfile.str().c_str(), "RECREATE");
  cout << "Text output file1: " << endl << textfile1.str() << endl;




    ostringstream mapcorr1;
    mapcorr1 << outpath << "/diff_pan_plate_" << scan <<".png";

    ostringstream zcorr;
    zcorr << outpath << "/diff_pan_plate_zdistrib_" << scan << ".png";

   TCanvas *c0 = new TCanvas("map_s","map_s",1000,700);
   map_s->SetMarkerSize(0.8);
   map_s->Draw("colz1,text45");
   map_s->SetStats(kFALSE);
  // map_s->GetZaxis()->SetRangeUser(11.4, 11.7);
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
   //diff->GetZaxis()->SetRangeUser(11.4, 11.7);
   diff->Write();
   c4->Write(mapcorr1.str().c_str());
   media=distz_diff->GetMean(1);
   RMS=distz_diff->GetRMS(1);
   //min=distz_diff->GetMinimum(1);
   //max=distz_diff->GetMaximum(1);
   //Data << media  << " \t " << RMS << "\t" << max << "\t" << min << endl;
   Data <<"mean= " <<  media  <<"\t" << "RMS= " << RMS << endl;

   TCanvas *c15 = new TCanvas("diff5","diff5",1000,700);
   diff5->SetMarkerSize(0.8);
   diff5->Draw("colz1,text45");
   diff5->SetStats(kFALSE);
   //diff->GetZaxis()->SetRangeUser(11.4, 11.7);
   diff5->Write();
   media5=distz_diff5->GetMean(1);
   RMS5=distz_diff5->GetRMS(1);
   //min=distz_diff->GetMinimum(1);
   //max=distz_diff->GetMaximum(1);
   //Data << media  << " \t " << RMS << "\t" << max << "\t" << min << endl;
   Data5 <<"mean5= " <<  media5  <<"\t" << "RMS5= " << RMS5 << endl;

   TCanvas *c14 = new TCanvas("diff4","diff4",1000,700);
   diff4->SetMarkerSize(0.8);
   diff4->Draw("colz1,text45");
   diff4->SetStats(kFALSE);
   //diff->GetZaxis()->SetRangeUser(11.4, 11.7);
   diff4->Write();
   media4=distz_diff4->GetMean(1);
   RMS4=distz_diff4->GetRMS(1);
   //min=distz_diff->GetMinimum(1);
   //max=distz_diff->GetMaximum(1);
   //Data << media  << " \t " << RMS << "\t" << max << "\t" << min << endl;
   Data4 <<"mean4= " <<  media4  <<"\t" << "RMS4= " << RMS4 << endl;
   
   TCanvas *c13 = new TCanvas("diff3","diff3",1000,700);
   diff3->SetMarkerSize(0.8);
   diff3->Draw("colz1,text45");
   diff3->SetStats(kFALSE);
   //diff->GetZaxis()->SetRangeUser(11.4, 11.7);
   diff3->Write();
   media3=distz_diff3->GetMean(1);
   RMS3=distz_diff3->GetRMS(1);
   //min=distz_diff->GetMinimum(1);
   //max=distz_diff->GetMaximum(1);
   //Data << media  << " \t " << RMS << "\t" << max << "\t" << min << endl;
   Data3 <<"mean3= " <<  media3  <<"\t" << "RMS3= " << RMS3 << endl;

   TCanvas *c12 = new TCanvas("diff2","diff2",1000,700);
   diff2->SetMarkerSize(0.8);
   diff2->Draw("colz1,text45");
   diff2->SetStats(kFALSE);
   //diff->GetZaxis()->SetRangeUser(11.4, 11.7);
   diff2->Write();
   media2=distz_diff2->GetMean(1);
   RMS2=distz_diff2->GetRMS(1);
   //min=distz_diff->GetMinimum(1);
   //max=distz_diff->GetMaximum(1);
   //Data << media  << " \t " << RMS << "\t" << max << "\t" << min << endl;
   Data2 <<"mean2= " <<  media2  <<"\t" << "RMS2= " << RMS2 << endl;

   TCanvas *c11 = new TCanvas("diff1","diff1",1000,700);
   diff1->SetMarkerSize(0.8);
   diff1->Draw("colz1,text45");
   diff1->SetStats(kFALSE);
   //diff->GetZaxis()->SetRangeUser(11.4, 11.7);
   diff1->Write();
   media1=distz_diff1->GetMean(1);
   RMS1=distz_diff1->GetRMS(1);
   //min=distz_diff->GetMinimum(1);
   //max=distz_diff->GetMaximum(1);
   //Data << media  << " \t " << RMS << "\t" << max << "\t" << min << endl;
   Data1 <<"mean1= " <<  media1  <<"\t" << "RMS1= " << RMS1 << endl;


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

  myroot.Close();
   return; 
}





















