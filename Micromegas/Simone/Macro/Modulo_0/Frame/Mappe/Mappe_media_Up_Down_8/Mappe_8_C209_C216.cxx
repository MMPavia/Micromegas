#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Modulo_0/LinearH/Prime_misure_modulo_0/Frame_modulo_0/"
#define outpath "/home/atlas/Micromegas/Simone/Output/Modulo_0/LinearH/Mappe_frame/Mappe_9_frame/"

void Mappe_8_C209_C216 (string scan1, string scan2 ,string scan3,string scan4,string scan5, string scan6, string scan7,string scan8,string scan9,string scan10,
                        string scan11, string scan12,string scan13,string scan14,string scan15, string scan16// , string scan17,string scan18
			 ){
// style option
// gROOT->LoadMacro("AtlasUtils.C");   // myText
//SetAtlasStyle(); 
 gStyle->SetPalette(1);
 gStyle->SetOptStat(111111);
 gStyle->SetPaintTextFormat(".0f");
// gStyle->SetOptTitle(0);
 gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
 gStyle->SetNumberContours(100);

// file containing the data
   ostringstream file1;
   file1 << inpath  << scan1 ;  
   ostringstream file2;
   file2 << inpath  << scan2 ;

   ostringstream file3;
   file3 << inpath  << scan3 ;
   ostringstream file4;
   file4 << inpath  << scan4 ;

   ostringstream file5;
   file5 << inpath  << scan5 ;
   ostringstream file6;
   file6 << inpath  << scan6 ;

   ostringstream file7;
   file7 << inpath  << scan7 ;
   ostringstream file8;
   file8 << inpath  << scan8 ;

   ostringstream file9;
   file9 << inpath  << scan9 ;
   ostringstream file10;
   file10 << inpath  << scan10 ;

   ostringstream file11;
   file11 << inpath  << scan11 ;
   ostringstream file12;
   file12 << inpath  << scan12 ;

   ostringstream file13;
   file13 << inpath  << scan13 ;
   ostringstream file14;
   file14 << inpath  << scan14 ;

   ostringstream file15;
   file15 << inpath  << scan15 ;
   ostringstream file16;
   file16 << inpath  << scan16 ;
/*
   ostringstream file17;
   file17 << inpath  << scan17 ;
   ostringstream file18;
   file18 << inpath  << scan18 ;
*/

// histograms binning info
  int nxbin=12;
  int nybin=8;
  float xmin = 0;
  float xmax = 12;
  float ymin =208.5;
  float ymax =216.5;

//  2d map of the table surface
   TH2F *tmap1= new TH2F("Frame 209","Side's frame measurements", nxbin, xmin, xmax, nybin, ymin, ymax );
//   tmap1->SetTitleOffset(2);
   tmap1->GetXaxis()->SetTitle("X (cm)");
//   tmap1->GetZaxis()->SetRangeUser(-170,170);
   tmap1->GetYaxis()->SetTitle("Frame number");
   tmap1->GetZaxis()->SetTitle("Z (#mum)");
/*
   TH2F *tmap2= new TH2F("Frame 210","Frame 210", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap2->GetXaxis()->SetTitle("X (cm)");
   tmap2->GetYaxis()->SetTitle("Y (x)");
   tmap2->GetZaxis()->SetTitle("Z (#mum)");
   tmap2->GetZaxis()->SetRangeUser(-70,70);
   TH2F *tmap3= new TH2F("Frame 211","Frame 211", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap3->GetXaxis()->SetTitle("X (cm)");
   tmap3->GetYaxis()->SetTitle("Y (x)");
   tmap3->GetZaxis()->SetTitle("Z (#mum)");
   tmap3->GetZaxis()->SetRangeUser(-70,70);
   TH2F *tmap4= new TH2F("Frame 212","Frame 212", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap4->GetXaxis()->SetTitle("X (cm)");
   tmap4->GetYaxis()->SetTitle("Y (x)");
   tmap4->GetZaxis()->SetTitle("Z (#mum)");
   tmap4->GetZaxis()->SetRangeUser(-70,70);
   TH2F *tmap5= new TH2F("Frame 213","Frame 213", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap5->GetXaxis()->SetTitle("X (cm)");
   tmap5->GetYaxis()->SetTitle("Y (x)");
   tmap5->GetZaxis()->SetTitle("Z (#mum)");
   tmap5->GetZaxis()->SetRangeUser(-70,70);
   TH2F *tmap6= new TH2F("Frame 214","Frame 214", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap6->GetXaxis()->SetTitle("X (cm)");
   tmap6->GetYaxis()->SetTitle("Y (x)");
   tmap6->GetZaxis()->SetTitle("Z (#mum)");
   tmap6->GetZaxis()->SetRangeUser(-70,70);
   TH2F *tmap7= new TH2F("Frame 215","Frame 215", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap7->GetXaxis()->SetTitle("X (cm)");
   tmap7->GetYaxis()->SetTitle("Y (x)");
   tmap7->GetZaxis()->SetTitle("Z (#mum)");
   tmap7->GetZaxis()->SetRangeUser(-70,70);
   TH2F *tmap8= new TH2F("Frame 216","Frame 216", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap8->GetXaxis()->SetTitle("X (cm)");
   tmap8->GetYaxis()->SetTitle("Y (x)");
   tmap8->GetZaxis()->SetTitle("Z (#mum)");
   tmap8->GetZaxis()->SetRangeUser(-70,70);
   TH2F *tmap9= new TH2F("Frame 216","Frame 216", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap9->GetXaxis()->SetTitle("X (cm)");
   tmap9->GetYaxis()->SetTitle("Y (x)");
   tmap9->GetZaxis()->SetTitle("Z (#mum)");
*/
   cout << "histos created " << endl;

   Float_t x,y;
   Double_t z1,z2, coord, a, u;
   Int_t nmis1,nmis2,j;
   string date, time;


// Prima frame
	
   ifstream in1(file1.str().c_str());
   ifstream in2(file2.str().c_str());
   if (!in1 || in1.bad() )
   {
	cout << "file 1 non letto" << endl;
	return; 
   }

   if (!in2 || in2.bad() )
   {
        cout << "file 2 non letto" << endl;
        return; 
   }
   cout << "starting reading file " << endl;


   while (1) {
		
	in1 >> nmis1;
	in2 >> nmis2;
	
	if ( !in1.good() || in1.eof()) break; 
        if ( !in2.good() || in2.eof()) break;

	cout <<"nmis " << nmis1 << endl;
		
		in1 >> z1;
		in2 >> z2;

		coord=(((z1+z2)/2)-26.000)*1000;
		x= nmis1-1;
		y= 209;
		tmap1->Fill(x, y, coord);
	
	cout << "x= " << x << endl;	
	cout << "nmis= " << nmis1 << endl;
	cout <<"coord= " <<coord << endl;	
	in1 >> date >> time;
	in2 >> date >> time;
 
	cout << "date " << date <<" time " << time << endl; 

   } 

   cout << "file completed "<< endl;

   in1.close();
   in2.close();

// Seconda frame

   in1.open(file3.str().c_str());
   in2.open(file4.str().c_str());
   if (!in1 || in1.bad() )
   {
	cout << "file 1 non letto" << endl;
	return; 
   }

   if (!in2 || in2.bad() )
   {
        cout << "file 2 non letto" << endl;
        return; 
   }
   cout << "starting reading file " << endl;


   while (1) {
		
	in1 >> nmis1;
	in2 >> nmis2;
	
	if ( !in1.good() || in1.eof()) break; 
        if ( !in2.good() || in2.eof()) break;

	cout <<"nmis " << nmis1 << endl;
		
		in1 >> z1;
		in2 >> z2;

		coord=(((z1+z2)/2)-26.000)*1000;
		x= nmis1-1;
		y= 210;
		tmap1->Fill(x, y, coord);
	
	cout << "x= " << x << endl;	
	cout << "nmis= " << nmis1 << endl;
	cout <<"coord= " <<coord << endl;	
	in1 >> date >> time;
	in2 >> date >> time;
 
	cout << "date " << date <<" time " << time << endl; 

   } 

   cout << "file completed "<< endl;

   in1.close();
   in2.close();

//terza frame

   in1.open(file5.str().c_str());
   in2.open(file6.str().c_str());
   if (!in1 || in1.bad() )
   {
	cout << "file 1 non letto" << endl;
	return; 
   }

   if (!in2 || in2.bad() )
   {
        cout << "file 2 non letto" << endl;
        return; 
   }
   cout << "starting reading file " << endl;


   while (1) {
		
	in1 >> nmis1;
	in2 >> nmis2;
	
	if ( !in1.good() || in1.eof()) break; 
        if ( !in2.good() || in2.eof()) break;

	cout <<"nmis " << nmis1 << endl;
		
		in1 >> z1;
		in2 >> z2;

		coord=(((z1+z2)/2)-26.000)*1000;
		x= nmis1-1;
		y= 211;
		tmap1->Fill(x, y, coord);
	
	cout << "x= " << x << endl;	
	cout << "nmis= " << nmis1 << endl;
	cout <<"coord= " <<coord << endl;	
	in1 >> date >> time;
	in2 >> date >> time;
 
	cout << "date " << date <<" time " << time << endl; 

   } 

   cout << "file completed "<< endl;

   in1.close();
   in2.close();

// quarta frame

   in1.open(file7.str().c_str());
   in2.open(file8.str().c_str());
   if (!in1 || in1.bad() )
   {
	cout << "file 1 non letto" << endl;
	return; 
   }

   if (!in2 || in2.bad() )
   {
        cout << "file 2 non letto" << endl;
        return; 
   }
   cout << "starting reading file " << endl;


   while (1) {
		
	in1 >> nmis1;
	in2 >> nmis2;
	
	if ( !in1.good() || in1.eof()) break; 
        if ( !in2.good() || in2.eof()) break;

	cout <<"nmis " << nmis1 << endl;
		
		in1 >> z1;
		in2 >> z2;

		coord=(((z1+z2)/2)-26.000)*1000;
		x= nmis1-1;
		y= 212;
		tmap1->Fill(x, y, coord);
	
	cout << "x= " << x << endl;	
	cout << "nmis= " << nmis1 << endl;
	cout <<"coord= " <<coord << endl;	
	in1 >> date >> time;
	in2 >> date >> time;
 
	cout << "date " << date <<" time " << time << endl; 

   } 

   cout << "file completed "<< endl;

   in1.close();
   in2.close();

// quinta frame

   in1.open(file9.str().c_str());
   in2.open(file10.str().c_str());
   if (!in1 || in1.bad() )
   {
	cout << "file 1 non letto" << endl;
	return; 
   }

   if (!in2 || in2.bad() )
   {
        cout << "file 2 non letto" << endl;
        return; 
   }
   cout << "starting reading file " << endl;


   while (1) {
		
	in1 >> nmis1;
	in2 >> nmis2;
	
	if ( !in1.good() || in1.eof()) break; 
        if ( !in2.good() || in2.eof()) break;

	cout <<"nmis " << nmis1 << endl;
		
		in1 >> z1;
		in2 >> z2;

		coord=(((z1+z2)/2)-26.000)*1000;
		x= nmis1-1;
		y= 213;
		tmap1->Fill(x, y, coord);
	
	cout << "x= " << x << endl;	
	cout << "nmis= " << nmis1 << endl;
	cout <<"coord= " <<coord << endl;	
	in1 >> date >> time;
	in2 >> date >> time;
 
	cout << "date " << date <<" time " << time << endl; 

   } 

   cout << "file completed "<< endl;

   in1.close();
   in2.close();

// sesta frame

   in1.open(file11.str().c_str());
   in2.open(file12.str().c_str());
   if (!in1 || in1.bad() )
   {
	cout << "file 1 non letto" << endl;
	return; 
   }

   if (!in2 || in2.bad() )
   {
        cout << "file 2 non letto" << endl;
        return; 
   }
   cout << "starting reading file " << endl;


   while (1) {
		
	in1 >> nmis1;
	in2 >> nmis2;
	
	if ( !in1.good() || in1.eof()) break; 
        if ( !in2.good() || in2.eof()) break;

	cout <<"nmis " << nmis1 << endl;
		
		in1 >> z1;
		in2 >> z2;

		coord=(((z1+z2)/2)-26.000)*1000;
		x= nmis1-1;
		y= 214;
		tmap1->Fill(x, y, coord);
	
	cout << "x= " << x << endl;	
	cout << "nmis= " << nmis1 << endl;
	cout <<"coord= " <<coord << endl;	
	in1 >> date >> time;
	in2 >> date >> time;
 
	cout << "date " << date <<" time " << time << endl; 

   } 

   cout << "file completed "<< endl;

   in1.close();
   in2.close();

// settima frame

   in1.open(file13.str().c_str());
   in2.open(file14.str().c_str());
   if (!in1 || in1.bad() )
   {
	cout << "file 1 non letto" << endl;
	return; 
   }

   if (!in2 || in2.bad() )
   {
        cout << "file 2 non letto" << endl;
        return; 
   }
   cout << "starting reading file " << endl;


   while (1) {
		
	in1 >> nmis1;
	in2 >> nmis2;
	
	if ( !in1.good() || in1.eof()) break; 
        if ( !in2.good() || in2.eof()) break;

	cout <<"nmis " << nmis1 << endl;
		
		in1 >> z1;
		in2 >> z2;

		coord=(((z1+z2)/2)-26.000)*1000;
		x= nmis1-1;
		y= 215;
		tmap1->Fill(x, y, coord);
	
	cout << "x= " << x << endl;	
	cout << "nmis= " << nmis1 << endl;
	cout <<"coord= " <<coord << endl;	
	in1 >> date >> time;
	in2 >> date >> time;
 
	cout << "date " << date <<" time " << time << endl; 

   } 

   cout << "file completed "<< endl;

   in1.close();
   in2.close();

// ottava frame

   in1.open(file15.str().c_str());
   in2.open(file16.str().c_str());
   if (!in1 || in1.bad() )
   {
	cout << "file 1 non letto" << endl;
	return; 
   }

   if (!in2 || in2.bad() )
   {
        cout << "file 2 non letto" << endl;
        return; 
   }
   cout << "starting reading file " << endl;


   while (1) {
		
	in1 >> nmis1;
	in2 >> nmis2;
	
	if ( !in1.good() || in1.eof()) break; 
        if ( !in2.good() || in2.eof()) break;

	cout <<"nmis " << nmis1 << endl;
		
		in1 >> z1;
		in2 >> z2;

		coord=(((z1+z2)/2)-26.000)*1000;
		x= nmis1-1;
		y= 216;
		tmap1->Fill(x, y, coord);
	
	cout << "x= " << x << endl;	
	cout << "nmis= " << nmis1 << endl;
	cout <<"coord= " <<coord << endl;	
	in1 >> date >> time;
	in2 >> date >> time;
 
	cout << "date " << date <<" time " << time << endl; 

   } 

   cout << "file completed "<< endl;

   in1.close();
   in2.close();

// nona frame
/*
   in1.open(file17.str().c_str());
   in2.open(file18.str().c_str());
   if (!in1 || in1.bad() )
   {
	cout << "file 1 non letto" << endl;
	return; 
   }

   if (!in2 || in2.bad() )
   {
        cout << "file 2 non letto" << endl;
        return; 
   }
   cout << "starting reading file " << endl;


   while (1) {
		
	in1 >> nmis1;
	in2 >> nmis2;
	
	if ( !in1.good() || in1.eof()) break; 
        if ( !in2.good() || in2.eof()) break;

	cout <<"nmis " << nmis1 << endl;
		
		in1 >> z1;
		in2 >> z2;

		coord=(((z1+z2)/2)-10.000)*1000;
		x= nmis1-1;
		y= 1;
		tmap9->Fill(x, y, coord);
        
                in1 >> z1;
                in2 >> z2;
                
                coord= (((z1+z2)/2)-10.000)*1000;
                y= 2;
                tmap9->Fill(x, y, coord);

                in1 >> z1;
                in2 >> z2;
                
                coord= (((z1+z2)/2)-10.000)*1000;
                y= 3;
                tmap9->Fill(x, y, coord);		
	
	cout << "x= " << x << endl;	
	cout << "nmis= " << nmis1 << endl;
	cout <<"coord= " <<coord << endl;	
	in1 >> date >> time;
	in2 >> date >> time;
 
	cout << "date " << date <<" time " << time << endl; 

   } 

   cout << "file completed "<< endl;

   in1.close();
   in2.close();


*/


//  controllo per buchi

   Int_t binx = tmap1->GetNbinsX();
   Int_t biny = tmap1->GetNbinsY();
   cout << "numero di bin in y= " << biny << endl;
   cout << "numero di bin in x= " << binx << endl;


   for (int i=1; i<=biny; i++){
   	for (int j=1; j<=binx; j++){

	        cout << " i= " << i << endl << " j= " << j << endl;
        	double t = tmap1->GetBinContent(j,i);
		cout << "coord = " << t << endl;

    
     }
   }
	
/*	
	// create a root file for the histograms; 
   ostringstream rootfile;
   rootfile  << outpath <<"myout_"<<scan1 <<".root";
   TFile myroot(rootfile.str().c_str(), "RECREATE");
   cout << "Root output file: " << endl << rootfile.str() << endl;

*/




   ostringstream Smap;
   Smap << outpath << "/map_costa_"<< scan1 <<".png";



    TCanvas *c0 = new TCanvas("c0", "c0", 1000,700);
//    c0->Divide(3,3);
//    c0->cd(2);
    tmap1->SetMarkerSize(0.8);
    tmap1->Draw("colz,text45");
    tmap1->SetStats(kFALSE);
    tmap1->Write();
/*
    c0->cd(3);
    tmap2->SetMarkerSize(0.8);
    tmap2->Draw("colz,text45");
    tmap2->SetStats(kFALSE);
    tmap2->Write();
    c0->cd(4);
    tmap3->SetMarkerSize(0.8);
    tmap3->Draw("colz,text45");
    tmap3->SetStats(kFALSE);
    tmap3->Write();
    c0->cd(5);
    tmap4->SetMarkerSize(0.8);
    tmap4->Draw("colz,text45");
    tmap4->SetStats(kFALSE);
    tmap4->Write();
    c0->cd(6);
    tmap5->SetMarkerSize(0.8);
    tmap5->Draw("colz,text45");
    tmap5->SetStats(kFALSE);
    tmap5->Write();
    c0->cd(7);
    tmap6->SetMarkerSize(0.8);
    tmap6->Draw("colz,text45");
    tmap6->SetStats(kFALSE);
    tmap6->Write();
    c0->cd(8);
    tmap7->SetMarkerSize(0.8);
    tmap7->Draw("colz,text45");
    tmap7->SetStats(kFALSE);
    tmap7->Write();
    c0->cd(9);
    tmap8->SetMarkerSize(0.8);
    tmap8->Draw("colz,text45");
    tmap8->SetStats(kFALSE);
    tmap8->Write();
*/ 
    c0->Print(Smap.str().c_str());
	
 
	//myroot.Close();


  return;
}
