#include <stdio.h>
#include <string>
#include <vector>
//#include "AtlasStyle.C"

//#define inpath "/home/atlas/Micromegas/M0Data/honeycomb/"
//#define outpath "/home/atlas/Micromegas/M0Data/root_plot/honeycomb/"

// Macro per graficare i valori misurati e confrontarli con quelli di roma

#define inpath "/home/atlas/Micromegas/Simone/Input/Modulo_0/Differenze_R_P/"
#define outpath "/home/atlas/Micromegas/Simone/Output/Modulo_0/Differenze_Roma_Pavia/"



void ave_imatec_22 (string scan){

  // style option
   gROOT->LoadMacro("AtlasUtils.C");   // myText
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".3f"); 
   gStyle->SetOptTitle(0);
   gStyle->SetPadLeftMargin(0.15);   
   gStyle->SetNumberContours(100);


  // file containing the data
  ostringstream file1;
  file1 << inpath  << scan ;  //compongo nome del file 
//  scan.erase(scan.end()-4,scan.end());

  Int_t j=0;
  Int_t n=5; 
  double foil[n];
  double  aveR[n],errR[n],aveP[n],errP[n];

 //for(j=0; j<n; j++){
 // cout << foil[j] <<"\t"<< aveR[j] <<"\t"<< errR[j]<<"\t"<< aveP[j] <<"\t"<< errP[j]<<"\t"<<endl;
 // }
 // j=0;

 // reading the file
  ifstream in(file1.str().c_str());
  if (!in || in.bad()) return; // sanity check

  cout << "starting reading file " << endl;
  // looping on the file lines
  while (1) {

    if (!in.good()|| in.eof()) break; // another sanity check

    // actual reading
    in >> foil[j] >> aveR[j] >> errR[j] >> aveP[j] >> errP[j]; 
    
    aveR[j] = aveR[j]/1000;
    aveP[j] = aveP[j]/1000; 
//    errR[j] = errR[j]*3;
//    errP[j] = errP[j]*3;

    cout << foil[j] <<"\t"<< aveR[j] <<"\t"<< errR[j]<<"\t"<< aveP[j] <<"\t"<< errP[j]<<"\t"<<endl; 

    j++;

    if (j==n) break;   

    if (!in.good()|| in.eof()) break; // another sanity check 
  } // end while

  cout << "file completed "<< endl;


 // close the input file
  in.close();

  TMultiGraph *mg = new TMultiGraph();
  mg->SetMinimum(9.900);
  mg->SetMaximum(10.100);

  TGraph * g1 = new TGraphErrors(n, foil, aveR, errR, errR); 
//  g1->GetYaxis()->SetRangeUser(9.900,10.080);
  g1->SetMarkerStyle(21);
  g1->SetMarkerColor(kRed);
  //g1->GetXaxis()->SetTitle("Foil Number");
  //g1->GetYaxis()->SetTitle("Thickness (mm)");
  mg->Add(g1);

  TGraph * g2 = new TGraphErrors(n, foil, aveP, errP, errP); 
  g2->SetMarkerStyle(22);
  g2->SetMarkerColor(kBlue);
//  g2->GetYaxis()->SetRangeUser(9.900,10.080);
  //g2->GetXaxis()->SetTitle("Foil Number");
  //g2->GetYaxis()->SetTitle("Thickness (mm)");
  mg->Add(g2);
  
/*
  TGraph * g3 = new TGraph(n,foil, max); 
  g3->SetMarkerStyle(23);
  g3->SetMarkerColor(kBlue);
  //g3->GetXaxis()->SetTitle("Foil Number");
  //g3->GetYaxis()->SetTitle("Thickness (mm)");
  mg->Add(g3);
*/

  mg->SetTitle("Frame Average Thickness: Roma LH - Pavia LH; Frame number; Thickness (mm)");

  ostringstream Sdistrib;
  Sdistrib  << outpath << "/averageThickness_pv_blocchetto_tastatore_up.pdf";  


  // create a canvas for each plot and drawing with different options
  TCanvas *c0 = new TCanvas("c0", "c0", 1000,700);
//  c0->Setgrid();
  TPad *pad2 = new TPad("pad2","The pad with the histogram",0.03,0.02,0.97,0.9,0);
  pad2->Draw();
  TPaveLabel *title2 = new TPaveLabel(0.12,0.91,0.52,0.96,"Frame Average Thickness (UP): Gauge block - No block");
  title2->SetFillColor(kBlue-10);
  title2->SetTextFont(42);
  title2->Draw();		    
  pad2->cd();
  mg->Draw("ap");
 
  TLegend *leg = new TLegend(.75,.80,.95,.95);
  leg->SetFillColor(0);
//  leg->SetHeader("legend");
  leg->AddEntry(g1, "Gauge block", "lp");
  leg->AddEntry(g2, "No block", "lp");
  leg->Draw();

  TLine *l1 = new TLine(207.5,10.060,224.5,10.060);
  TLine *l2 = new TLine(207.5,9.940,224.5,9.940);
  l1->SetLineColor(kRed); 
  l2->SetLineColor(kRed);
  l1->SetLineWidth(3); 
  l2->SetLineWidth(3); 
  l1->Draw(); l2->Draw();
  c0->Print(Sdistrib.str().c_str());


  return;
}
