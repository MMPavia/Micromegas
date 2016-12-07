#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Modulo_0/LinearH/Prime_misure_modulo_0/Frame_modulo_0/"
#define outpath "/home/atlas/Micromegas/Simone/Output/Modulo_0/LinearH/Mappe_frame/Mappe_9_frame/"

void mapping_media (string scan1, string scan2,string scan3,string scan4,string scan5, string scan6, string scan7,string scan8,string scan9,string scan10,
			string scan11, string scan12,string scan13,string scan14,string scan15, string scan16, string scan17,string scan18){

// style option
 gROOT->LoadMacro("AtlasUtils.C");   // myText
//SetAtlasStyle(); 
 gStyle->SetPalette(1);
 gStyle->SetOptStat(111111);
 gStyle->SetPaintTextFormat(".3f");
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

   TH1F *tmedia = new TH1F("media","media", nybin*nxbin,9.9, 10.1 );
   TH1F *tmedia1 = new TH1F("media punto sx","media", nybin*nxbin,9.9, 10.1 );
   TH1F *tmedia2 = new TH1F("media punto centrale","media", nybin*nxbin,9.9, 10.1 );
   TH1F *tmedia3 = new TH1F("media punto dx","media", nybin*nxbin,9.9, 10.1 );
   TH1F *tsigma2 = new TH1F("sigma","sigma", nybin*nxbin,0, 5 );

   TH2F *tmap1= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   TH2F *tmap2= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   TH2F *tmap3= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   TH2F *tmap4= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   TH2F *tmap5= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   TH2F *tmap= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );   
   tmap->GetXaxis()->SetTitle("X (cm)");
   tmap->GetYaxis()->SetTitle("Y (x)");
   tmap->GetZaxis()->SetTitle("Z (#mum)");

   TH2F *tsigma= new TH2F("sigma","sigma", nxbin, xmin, xmax, nybin, ymin, ymax );
   tsigma->GetXaxis()->SetTitle("X (cm)");
   tsigma->GetYaxis()->SetTitle("Y (x)");
   tsigma->GetZaxis()->SetTitle("Sigma (#mum)");

   TH2F *tmap11= new TH2F("1-mu","1-mu", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap11->GetXaxis()->SetTitle("X (cm)");
   tmap11->GetYaxis()->SetTitle("Y (x)");
   tmap11->GetZaxis()->SetTitle("Z (#mum)");
   TH2F *tmap12= new TH2F("2-mu","2-mu", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap12->GetXaxis()->SetTitle("X (cm)");
   tmap12->GetYaxis()->SetTitle("Y (x)");
   tmap12->GetZaxis()->SetTitle("Z (#mum)");
   TH2F *tmap13= new TH2F("3-mu","3-mu", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap13->GetXaxis()->SetTitle("X (cm)");
   tmap13->GetYaxis()->SetTitle("Y (x)");
   tmap13->GetZaxis()->SetTitle("Z (#mum)");
   TH2F *tmap14= new TH2F("4-mu","4-mu", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap14->GetXaxis()->SetTitle("X (cm)");
   tmap14->GetYaxis()->SetTitle("Y (x)");
   tmap14->GetZaxis()->SetTitle("Z (#mum)");
   TH2F *tmap15= new TH2F("5-mu","5-mu", nxbin, xmin, xmax, nybin, ymin, ymax );
   tmap15->GetXaxis()->SetTitle("X (cm)");
   tmap15->GetYaxis()->SetTitle("Y (x)");
   tmap15->GetZaxis()->SetTitle("Z (#mum)");

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
		x=(nmis-1)*10;
		coord=( z-10.00);

                cntmedia1= cntmedia1 +coord;
	
		tmap1->Fill(x,y,coord);
        
	}
		
		
	in >> date >> time; 
	cout << "date " << date <<" time " << time << endl; 

		
   }
	
   cout << "file completed "<< endl;

   
   in.close();


// Lettura secondo file

   ostringstream file2;
   file2 << inpath  << scan2 ;

  ifstream in2(file2.str().c_str());



   if (!in2 || in2.bad() )
   {
        cout << "file non letto" << endl;
        return; 
   }

   cout << "starting reading file " << endl;



        
   while (1) {

        in2 >> nmis;

        if ( !in2.good() || in2.eof()) break;  

        cout <<"nmis " << nmis << endl;


        for (j=1;j<4;j++)
        {

                in2 >> z ;
                y=j;
                x=(nmis-1)*10;
                coord= ( z-10.00);

                cntmedia2= cntmedia2 +coord;

                tmap2->Fill(x,y,coord);

        }


        in2 >> date >> time;
        cout << "date " << date <<" time " << time << endl;


   } 

   cout << "file completed "<< endl;

   
   in2.close();



// lettura terzo file
 
   ostringstream file3;
   file3 << inpath  << scan3 ;


    ifstream in3(file3.str().c_str());



   if (!in3 || in3.bad() )
   {
        cout << "file non letto" << endl;
        return; 
   }

   cout << "starting reading file " << endl;



        
   while (1) {

        in3 >> nmis;

        if ( !in3.good() || in3.eof()) break;  

        cout <<"nmis " << nmis << endl;


        for (j=1;j<4;j++)
        {

                in3 >> z ;
                y=j;
                x=(nmis-1)*10;
                coord=( z-10.00);

                cntmedia3= cntmedia3 +coord;

                tmap3->Fill(x,y,coord);

        }


        in3 >> date >> time;
        cout << "date " << date <<" time " << time << endl;


   } 

   cout << "file completed "<< endl;

   
   in3.close();


//lettura quarto file

   ostringstream file4;
   file4 << inpath  << scan4 ;


   ifstream in4(file4.str().c_str());



   if (!in4 || in4.bad() )
   {
        cout << "file non letto" << endl;
        return; 
   }

   cout << "starting reading file " << endl;



        
   while (1) {

        in4 >> nmis;

        if ( !in4.good() || in4.eof()) break;  

        cout <<"nmis " << nmis << endl;


        for (j=1;j<4;j++)
        {

                in4 >> z ;
                y=j;
                x=(nmis-1)*10;
                coord=( z-10.00);
                cntmedia4= cntmedia4 +coord;

                tmap4->Fill(x,y,coord);

        }


        in4 >> date >> time;
        cout << "date " << date <<" time " << time << endl;


   } 

   cout << "file completed "<< endl;

   
   in4.close();



//lettura quinto file


   ostringstream file5;
   file5 << inpath  << scan5 ;


  ifstream in5(file5.str().c_str());



   if (!in5 || in5.bad() )
   {
        cout << "file non letto" << endl;
        return; 
   }

   cout << "starting reading file " << endl;



        
   while (1) {

        in5 >> nmis;

        if ( !in5.good() || in5.eof()) break;  

        cout <<"nmis " << nmis << endl;


        for (j=1;j<4;j++)
        {

                in5 >> z ;
                y=j;
                x=(nmis-1)*10;
                coord= ( z-10.00);
		cntmedia5= cntmedia5 +coord;

                tmap5->Fill(x,y,coord);

        }


        in5 >> date >> time;
        cout << "date " << date <<" time " << time << endl;


   } 

   cout << "file completed "<< endl;

   
   in5.close();



//  controllo per buchi

   Int_t binx = tmap1->GetNbinsX();
   Int_t biny = tmap1->GetNbinsY();

   double a,b,c,d,e,t,k,l,m,n,o,media2;
   double rms1, rms2,rms3,rms4,rms5,rms;

   for (int i=1; i<=biny; i++){
   	for (int j=0; j<binx; j++){

	        cout << " i " << i << " j " << j << endl;
        	a = tmap1->GetBinContent(j+1,i);
		b = tmap2->GetBinContent(j+1,i);
		c = tmap3->GetBinContent(j+1,i);
		d = tmap4->GetBinContent(j+1,i);
		e = tmap5->GetBinContent(j+1,i);
	
		media =((a+b+c+d+e)/5)*1000;
		media2=((a+b+c+d+e)/5)+10;
		t=(j)*10;
		tmap->Fill(t,i,media);
		tmedia->Fill(media);



		
		k = a*1000-media;
                l = b*1000-media;
                m = c*1000-media;
                n = d*1000-media;
                o = e*1000-media;

		tmap11->Fill(t,i,k);
		tmap12->Fill(t,i,l);
		tmap13->Fill(t,i,m);
		tmap14->Fill(t,i,n);
		tmap15->Fill(t,i,o);

//calcolo mappa sigma media


		rms= sqrt((k*k + l*l + m*m + n*n +o*o)/4);
                tsigma->Fill(t,i,rms);
		tsigma2->Fill(rms);


//Riempimento array per Graph media

		if(i==1){
		sx[j]=media;
		cout << "sx["<<j<<"]=  "<<sx[j]<<endl;
		}

		if(i==2){
		cn[j]=media;
                cout << "cn["<<j<<"]=  "<<cn[j]<<endl;
		}

		if(i==3){
		dx[j]=media;
		cont[j]=t;
                cout << "dx["<<j<<"]=  "<<dx[j]<<endl;
		cout << "count["<<j<<"]=  "<<cont[j]<<endl;
		}


		cout << "Y= " << i << endl;
                cout << "X= " << t << endl;
                cout << "media= " << media << endl;
		cout << "a= " << a << endl;
                cout << "k= " << k << endl;
                cout << "l= " << l << endl;
                cout << "m= " << m << endl;
                cout << "n= " << n << endl;
                cout << "o= " << o << endl;
 
   
     }
   }

                cout << "casella 8-1= " << tsigma->GetBinContent(8,1)<< endl;
                cout << "valore-media 1 8-1= " << tmap11->GetBinContent(8,1)<< endl;  
                cout << "valore-media 2 8-1= " << tmap12->GetBinContent(8,1)<< endl;  
                cout << "valore-media 3 8-1= " << tmap13->GetBinContent(8,1)<< endl;  
                cout << "valore-media 4 8-1= " << tmap14->GetBinContent(8,1)<< endl;  
                cout << "valore-media 5 8-1= " << tmap15->GetBinContent(8,1)<< endl;  
	
//riempimento 3 istogrammi media a punti

  	 for(int i=0; i<12; i++){
		tmedia1->Fill(sx[i]+10);
       		tmedia2->Fill(cn[i]+10);
  		tmedia3->Fill(dx[i]+10);
   	}




//Riempimento Graph media
    
	        cout << "dx[0]=  "<<dx[0]<<endl;
                cout << "count[1]=  "<<cont[1]<<endl;
                cout << "count[12]=  "<<cont[12]<<endl;
                cout << "sx[0]=  "<<sx[0]<<endl;

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



/*
	
	// create a root file for the histograms; 
   ostringstream rootfile;
   rootfile  << outpath <<"myout_"<<scan1<<scan2<<scan3<<scan4<<scan5 <<".root";
   TFile myroot(rootfile.str().c_str(), "RECREATE");
   cout << "Root output file: " << endl << rootfile.str() << endl;

*/

   ostringstream Smm,Ssigma, Ssigma2,Smedia,Smedia1,Smedia2,Smedia3 ,Smap,Smap1,Smap2,Smap3,Smap4,Smap5,SmapG;
Smap << outpath << "/map_"<< scan1<<scan2<<scan3<<scan4<<scan5 <<".png";
Smap1 << outpath << "/map_"<< scan1 <<".png";
Smap2 << outpath << "/map_"<< scan2 <<".png";
Smap3 << outpath << "/map_"<< scan3 <<".png";
Smap4 << outpath << "/map_"<< scan4 <<".png";
Smap5 << outpath << "/map_"<< scan5 <<".png";
Smedia << outpath << "/map_media.png";
Smedia1 << outpath << "/map_media1.png";
Smedia2 << outpath << "/map_media2.png";
Smedia3 << outpath << "/map_media3.png";
SmapG << outpath << "/Graph_media.png";
Smm << outpath << "/Graph_media_media.png";
Ssigma << outpath << "/map_sigma.png";
Ssigma2 << outpath << "/map_sigma36.png";

	// create canvas

    TCanvas *c0 = new TCanvas("c0", "c0", 1000,700);
    tsigma->SetMarkerSize(0.8);
    tsigma->Draw("colz,text45");
    tsigma->SetStats(kFALSE);
    tsigma->Write();
    c0->Print(Ssigma.str().c_str());

/*

    TCanvas *c66 = new TCanvas();
    tsigma2->Draw();
    tsigma2->Write();
    c66->Print(Ssigma2.str().c_str());

 

    TCanvas *c0 = new TCanvas("c0", "c0", 1000,700);
    tmap->SetMarkerSize(0.8);
    tmap->Draw("colz,text45");
    tmap->SetStats(kFALSE);
    tmap->Write();
    c0->Print(Smap.str().c_str());
	


    TCanvas *c1 = new TCanvas("c0", "c0", 1000,700);
    tmap11->SetMarkerSize(0.8);
    tmap11->Draw("colz,text45");
    tmap11->SetStats(kFALSE);
    tmap11->Write();
    c1->Print(Smap1.str().c_str());



    TCanvas *c2 = new TCanvas("c0", "c0", 1000,700);
    tmap12->SetMarkerSize(0.8);
    tmap12->Draw("colz,text45");
    tmap12->SetStats(kFALSE);
    tmap12->Write();
    c2->Print(Smap2.str().c_str());
 
    TCanvas *c3 = new TCanvas("c0", "c0", 1000,700);
    tmap13->SetMarkerSize(0.8);
    tmap13->Draw("colz,text45");
    tmap13->SetStats(kFALSE);
    tmap13->Write();
    c3->Print(Smap3.str().c_str());

    TCanvas *c4 = new TCanvas("c0", "c0", 1000,700);
    tmap14->SetMarkerSize(0.8);
    tmap14->Draw("colz,text45");
    tmap14->SetStats(kFALSE);
    tmap14->Write();
    c4->Print(Smap4.str().c_str());

    TCanvas *c5 = new TCanvas("c0", "c0", 1000,700);
    tmap15->SetMarkerSize(0.8);
    tmap15->Draw("colz,text45");
    tmap15->SetStats(kFALSE);
    tmap15->Write();
    c5->Print(Smap5.str().c_str());



  
    TCanvas *c6 = new TCanvas();
    tmedia->Draw();
    tmedia->Write();
    c6->Print(Smedia.str().c_str());

    TCanvas *c7 = new TCanvas();
    tmedia1->Draw();
    tmedia1->Write();
    c7->Print(Smedia1.str().c_str());


    TCanvas *c8 = new TCanvas();
    tmedia2->Draw();
    tmedia2->Write();
    c8->Print(Smedia2.str().c_str());


    TCanvas *c9 = new TCanvas();
    tmedia3->Draw();
    tmedia3->Write();
    c9->Print(Smedia3.str().c_str());





	//Creazione canvas per Graph
	
         TMultiGraph *mg = new TMultiGraph();



         mg->Add(gr1); gr1->SetTitle("Dx")  ;            gr1->SetLineWidth(3); gr1->SetLineColor(kRed);
         mg->Add(gr2); gr2->SetTitle("Centro");          gr2->SetLineWidth(3); gr2->SetLineColor(kBlue);
         mg->Add(gr3); gr3->SetTitle("Sx")  ;            gr3->SetLineWidth(3); gr3->SetLineColor(kGreen);



        TCanvas *cG = new TCanvas("c1","Graph Draw Options",200,10,600,400);
        mg->Draw("AC*");


         leg = new TLegend(0.1,0.7,0.48,0.9);
         leg->SetHeader("Spessore medio frame");
         leg->AddEntry(gr1,"Misura destra");
         leg->AddEntry(gr2,"Misura centrale");
         leg->AddEntry(gr3,"Misura sinistra");
         leg->Draw();  
   	 cG->Print(SmapG.str().c_str());

*/

/*
         TMultiGraph *mg = new TMultiGraph();



         mg->Add(gr8); gr8->SetTitle("Media 5 misure")  ;            gr1->SetLineWidth(3); gr1->SetLineColor(kRed);



        TCanvas *cG = new TCanvas("c1","Graph Draw Options",200,10,600,400);
        mg->Draw("AC*");


         leg = new TLegend(0.1,0.7,0.48,0.9);
         leg->SetHeader("Andamento della media");
         leg->Draw();  
         cG->Print(Smm.str().c_str());

*/

//  myroot.Close();


  return;
}
