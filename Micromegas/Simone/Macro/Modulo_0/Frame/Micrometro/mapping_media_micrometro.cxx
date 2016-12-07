#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Frame_Micrometro/"
#define outpath "/home/atlas/Micromegas/Simone/Output/Larghezza_frame/Largh_frame_micrometro/"


//Macro per il mapping dello spessore delle frame (3 punti unica misura)

void mapping_media_micrometro (string scan1){

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
   file1 << inpath  << scan1 ;  

// histograms binning info
  int nxbin=12;
  int nybin=3;
  float xmin = 0;
  float xmax = 120;
  float ymin = 1;
  float ymax =4;	

//  2d map of the table surface

   TH1F *tmedia = new TH1F("micrometro","micrometro", nybin*nxbin,9.95, 10.05 );

   TH2F *tmap1= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap1->GetXaxis()->SetTitle("X (cm)");
   tmap1->GetYaxis()->SetTitle("Y (x)");
   tmap1->GetZaxis()->SetTitle("Z (#mum)");

   TH2F *tsigma= new TH2F("sigma","sigma", nxbin, xmin, xmax, nybin, ymin, ymax );
   tsigma->GetXaxis()->SetTitle("X (cm)");
   tsigma->GetYaxis()->SetTitle("Y (x)");
   tsigma->GetZaxis()->SetTitle("Sigma (#mum)");

   cout << "histos created " << endl;

//variables for reading the file
	
   Float_t x,y;
   Double_t z, coord, media;
   Int_t nmis,j;
   string date, time;
   Double_t sx[12],cn[12],dx[12],cont[12];
   Double_t cntmedia1=0,cntmedia2=0,cntmedia3=0,cntmedia4=0,cntmedia5=0;
	
// reading the file1
   ifstream in(file1.str().c_str());


	
   if (!in || in.bad() )
   {
	cout << "file non letto" << endl;
	return;
   }
	
   cout << "starting reading file " << endl;




   while (1) {
		
	in >> nmis;
		
	if ( !in.good() || in.eof()) break; 

	cout <<"nmis " << nmis << endl;
		

	for (j=1;j<4;j++)
	{		
		
		in >> z ;
		y=j;
		x=(nmis)*10;
		coord=( z-10.00)*1000;

                cntmedia1= cntmedia1 +coord;
	
		tmap1->Fill(x,y,coord);
        
	}
		
		
	in >> date >> time; 
	cout << "date " << date <<" time " << time << endl; 

		
   }
	
   cout << "file completed "<< endl;

   
   in.close();



//  controllo per buchi

   Int_t binx = tmap1->GetNbinsX();
   Int_t biny = tmap1->GetNbinsY();

   double a=0;
   double rmss;

   for (int i=0; i<binx; i++){
   	for (int j=1; j<=biny; j++){

	        cout << " i " << i << " j " << j << endl;
        	a = a + (tmap1->GetBinContent(i,j));
	
		media =((a)/3);
		tmedia->Fill(((tmap1->GetBinContent(i,j)/1000)+10));


//Riempimento array per Graph media

		if(j==1){
		sx[i]=((tmap1->GetBinContent(i,j)/1000)+10);
		cout << "sx["<<j<<"]=  "<<sx[j]<<endl;
		}

		if(j==2){
		cn[i]=((tmap1->GetBinContent(i,j)/1000)+10);
                cout << "cn["<<j<<"]=  "<<cn[j]<<endl;
		}

		if(j==3){
		dx[i]=((tmap1->GetBinContent(i,j)/1000)+10);
		cont[i]=i*10;
                cout << "dx["<<j<<"]=  "<<dx[j]<<endl;
		cout << "count["<<j<<"]=  "<<cont[j]<<endl;
		}


     }
   }


//Riempimento Graph media
    

	        TGraph *gr1 = new TGraph (12,cont,sx);
       		TGraph *gr2 = new TGraph (12,cont,cn);
       		TGraph *gr3 = new TGraph (12,cont,dx);


                cout << "dx[12]=  "<<dx[12]<<endl;
                cout << "count[1]=  "<<cont[1]<<endl;
                cout << "count[12]=  "<<cont[12]<<endl;
                cout << "sx[1]=  "<<sx[1]<<endl;


//debug
//
/* 
  binx = tmap->GetNbinsX();
   biny = tmap->GetNbinsY();
   cout << "numero di bin in y= " << biny << endl;
   cout << "numero di bin in x= " << binx << endl;


   for (int i=1; i<=biny; i++){
        for (int j=1; j<=binx; j++){

                cout << " i " << i << " j " << j << endl;
                t = tmap->GetBinContent(j,i);
                cout << "coord = " << t << endl;
	}
   }
*/



   ostringstream Smedia,Smap,SmapG;
Smap << outpath << "/map_Larghezza_micrometro.png";
Smedia << outpath << "/isto_media_micrometro.png";
SmapG << outpath << "/Graph_largh_micrometro.png";

	// create canvas




    TCanvas *c0 = new TCanvas("c0", "c0", 1000,700);
    tmap1->SetMarkerSize(0.8);
    tmap1->Draw("colz,text45");
    tmap1->SetStats(kFALSE);
    tmap1->Write();
    c0->Print(Smap.str().c_str());


  
    TCanvas *c1 = new TCanvas();
    tmedia->Draw();
    tmedia->Write();
    c1->Print(Smedia.str().c_str());


	//Creazione canvas per Graph
	
         TMultiGraph *mg = new TMultiGraph();

         mg->Add(gr1); gr1->SetTitle("Dx")  ;            gr1->SetLineWidth(3); gr1->SetLineColor(kRed);
         mg->Add(gr2); gr2->SetTitle("Centro");          gr2->SetLineWidth(3); gr2->SetLineColor(kBlue);
         mg->Add(gr3); gr3->SetTitle("Sx")  ;            gr3->SetLineWidth(3); gr3->SetLineColor(kGreen);

        TCanvas *cG = new TCanvas("c1","Graph Draw Options",200,10,600,400);
        mg->Draw("AC*");

         leg = new TLegend(0.1,0.7,0.48,0.9);
         leg->SetHeader("Andamento spessore (micrometro)");
         leg->AddEntry(gr1,"Bordo 1");
         leg->AddEntry(gr2,"Centro"); 
         leg->AddEntry(gr3,"Bordo 2");
	 leg->Draw();  
         mg->Print(SmapG.str().c_str());



//  myroot.Close();


  return;
}
