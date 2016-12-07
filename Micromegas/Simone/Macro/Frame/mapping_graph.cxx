#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Frame_3_punti/"
#define outpath "/home/atlas/Micromegas/Simone/Output/"

void mapping_graph (string scan1, string scan2,string scan3,string scan4,string scan5){

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
  Int_t ii=0;
// -10 2d map of the table surface

   TH1F *tmedia = new TH1F("media","media", nybin*nxbin,-50, 50 );
   TH1F *tmedia1 = new TH1F("media punto sx","media", nybin*nxbin,-50, 50 );
   TH1F *tmedia2 = new TH1F("media punto centrale","media", nybin*nxbin,-50, 50 );
   TH1F *tmedia3 = new TH1F("media punto dx","media", nybin*nxbin,-50, 50 );

   TH2F *tmap1= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   TH2F *tmap2= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   TH2F *tmap3= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   TH2F *tmap4= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   TH2F *tmap5= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );
   TH2F *tmap= new TH2F("map","map", nxbin, xmin, xmax, nybin, ymin, ymax );   
   tmap->GetXaxis()->SetTitle("X (cm)");
   tmap->GetYaxis()->SetTitle("Y (x)");
   tmap->GetZaxis()->SetTitle("Z (#mum)");

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
   Double_t std1[36],std2[36],std3[36],std4[36],std5[36],std;

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
		coord=( z);

                cntmedia1= cntmedia1 +coord;
		tmap1->Fill(x,y,coord);
    		std1[ii]=coord;    
        ii++;
	}
		
		
	in >> date >> time; 
	cout << "date " << date <<" time " << time << endl; 
	   cout << "std1["<<ii << "]= "<<std1[ii]<< endl;

	
   }
	
   cout << "file completed "<< endl;
        ii=0;
   
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
                coord= ( z);

                cntmedia2= cntmedia2 +coord;
                std2[ii]=coord;
                tmap2->Fill(x,y,coord);
        ii++;
        }


        in2 >> date >> time;
        cout << "date " << date <<" time " << time << endl;

   } 

   cout << "file completed "<< endl;
        ii=0;
   
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
                coord=( z);

                cntmedia3= cntmedia3 +coord;
                std3[ii]=coord;
                tmap3->Fill(x,y,coord);
        ii++;
        }


        in3 >> date >> time;
        cout << "date " << date <<" time " << time << endl;

   } 

   cout << "file completed "<< endl;
        ii=0;
   
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
                coord=( z);
                cntmedia4= cntmedia4 +coord;
                std4[ii]=coord;
                tmap4->Fill(x,y,coord);
        ii++;
        }


        in4 >> date >> time;
        cout << "date " << date <<" time " << time << endl;

   } 

   cout << "file completed "<< endl;
        ii=0;
   
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
                coord= ( z);
		cntmedia5= cntmedia5 +coord;
                std5[ii]=coord;

                tmap5->Fill(x,y,coord);
        ii++;
        }


        in5 >> date >> time;
        cout << "date " << date <<" time " << time << endl;

   } 

	ii=0;

   cout << "file completed "<< endl;

   
   in5.close();



//  controllo per buchi

   Int_t binx = tmap1->GetNbinsX();
   Int_t biny = tmap1->GetNbinsY();

   double a,b,c,d,e,t,k,l,m,n,o,media2;


   for (int i=1; i<=biny; i++){
   	for (int j=0; j<binx; j++){

	        cout << " i " << i << " j " << j << endl;
        	a = tmap1->GetBinContent(j,i);
		b = tmap2->GetBinContent(j,i);
		c = tmap3->GetBinContent(j,i);
		d = tmap4->GetBinContent(j,i);
		e = tmap5->GetBinContent(j,i);
	
		media =((a+b+c+d+e)/5) ;
		media2=((a+b+c+d+e)/5);
		t=(j-0)*10;
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
                cout << "k= " << k << endl;
                cout << "l= " << l << endl;
                cout << "m= " << m << endl;
                cout << "n= " << n << endl;
                cout << "o= " << o << endl;
 

   
     }
   }
	
//riempimento 3 istogrammi media a punti


   for(int i=0; i<12; i++){
	tmedia1->Fill(sx[i]);
        tmedia2->Fill(cn[i]);
        tmedia3->Fill(dx[i]);
   }



//Riempimento Graph media
                cout << "dx[12]=  "<<dx[12]<<endl;
                cout << "count[12]=  "<<cont[12]<<endl;
                cout << "sx[1]=  "<<sx[1]<<endl;

        TGraph *gr1 = new TGraph (12,cont,sx);
        TGraph *gr2 = new TGraph (12,cont,cn);
        TGraph *gr3 = new TGraph (12,cont,dx);

//Riempimento 5 Grafici media

         double numeri[5];
        numeri[0]=1;
        numeri[1]=2;
        numeri[2]=3;
        numeri[3]=4;
        numeri[4]=5;
 	
	 double media5[5];
	media5[0]=((cntmedia1)/36);
        media5[1]=((cntmedia2)/36);
        media5[2]=((cntmedia3)/36);
        media5[3]=((cntmedia4)/36);
        media5[4]=((cntmedia5)/36);
        TGraph *gr8 = new TGraph (5,numeri,media5);

	cout<< "media1= "<< media5[0]<<endl;
        cout<< "media2= "<< media5[1]<<endl;
        cout<< "media3= "<< media5[2]<<endl;
        cout<< "media4= "<< media5[3]<<endl;
        cout<< "media5= "<< media5[4]<<endl;
        cout<< "numeri1= "<< numeri[0]<<endl;
        cout<< "numeri2= "<< numeri[1]<<endl;
        cout<< "numeri3= "<< numeri[2]<<endl;
        cout<< "numeri4= "<< numeri[3]<<endl;
        cout<< "numeri5= "<< numeri[4]<<endl;

        cout<< "std11= "<< std1[0]<<endl;
        cout<< "std22= "<< std2[1]<<endl;
        cout<< "std33= "<< std3[2]<<endl;
        cout<< "std44= "<< std4[3]<<endl;
        cout<< "std55= "<< std5[4]<<endl;

        Double_t stdf1[36];
        Double_t stdf2[36];
        Double_t stdf3[36];
        Double_t stdf4[36];
        Double_t stdf5[36];

        for (ii=0;ii<36;ii++){
        stdf1[ii]=0;
        stdf2[ii]=0;
        stdf3[ii]=0;
        stdf4[ii]=0;
        stdf5[ii]=0;
        }


	for (ii=0;ii<36;ii++){
	stdf1[ii]= std1[ii]-media5[0];
        stdf2[ii]= std2[ii]-media5[1]; 
        stdf3[ii]= std3[ii]-media5[2];
        stdf4[ii]= std4[ii]-media5[3];
        stdf5[ii]= std5[ii]-media5[4];
	}

        cout<< "stdf1= "<< stdf1[0]<<endl;
        cout<< "stdf1= "<< stdf1[1]<<endl;
        cout<< "stdf1= "<< stdf1[2]<<endl;
        cout<< "stdf1= "<< stdf1[3]<<endl;
        cout<< "stdf1= "<< stdf1[4]<<endl;


        Double_t stdff[5]={0,0,0,0,0} ;

	for (ii=0;ii<36;ii++){
	stdff[0]=stdff[0]+(stdf1[ii]*stdf1[ii]);
        stdff[1]=stdff[1]+(stdf2[ii]*stdf2[ii]);
        stdff[2]=stdff[2]+(stdf3[ii]*stdf3[ii]);
        stdff[3]=stdff[3]+(stdf4[ii]*stdf4[ii]);
        stdff[4]=stdff[4]+(stdf5[ii]*stdf5[ii]);
	}

        cout<< "stdf1= "<< stdff[0]<<endl;
        cout<< "stdf2= "<< stdff[1]<<endl;
        cout<< "stdf3= "<< stdff[2]<<endl;
        cout<< "stdf4= "<< stdff[3]<<endl;
        cout<< "stdf5= "<< stdff[4]<<endl;


        stdff[0]=sqrt(stdff[0]/35);
        stdff[1]=sqrt(stdff[1]/35);
        stdff[2]=sqrt(stdff[2]/35);
        stdff[3]=sqrt(stdff[3]/35);
        stdff[4]=sqrt(stdff[4]/35);

        TGraph *gr9 = new TGraph (5,numeri,stdff);


        cout<< "stdf1= "<< stdff[0]<<endl;
        cout<< "stdf2= "<< stdff[1]<<endl;
        cout<< "stdf3= "<< stdff[2]<<endl;
        cout<< "stdf4= "<< stdff[3]<<endl;
        cout<< "stdf5= "<< stdff[4]<<endl;

//debag
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



	
	// create a root file for the histograms; 
   ostringstream rootfile;
   rootfile  << outpath <<"myout_"<<scan1<<scan2<<scan3<<scan4<<scan5 <<".root";
   TFile myroot(rootfile.str().c_str(), "RECREATE");
   cout << "Root output file: " << endl << rootfile.str() << endl;

   ostringstream Smediagraph,Ssigmagraph, Smm,Smedia,Smedia1,Smedia2,Smedia3 ,Smap,Smap1,Smap2,Smap3,Smap4,Smap5,SmapG;
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
Smediagraph << outpath << "/Mediagraph.png";
Ssigmagraph << outpath << "/Sigmagraph.png";

	// create canvas
/*

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



         TMultiGraph *mg1 = new TMultiGraph();



         mg1->Add(gr8); gr8->SetTitle("Media 5 misure")  ;            gr1->SetLineWidth(3); gr1->SetLineColor(kRed);



        TCanvas *cG1 = new TCanvas("c1","Graph Draw Options",200,10,600,400);
        mg1->Draw("AC*");


         leg = new TLegend(0.1,0.7,0.48,0.9);
         leg->SetHeader("Andamento della media");
         leg->Draw();  
         cG->Print(Smm.str().c_str());


    TCanvas *c11 = new TCanvas("c1","Graph Draw Options", 200,10,600,400);
    gr8->Draw("AC*");
    gr8->Print(Smediagraph.str().c_str());

*/
    TCanvas *c22 = new TCanvas("c1","Graph Draw Options", 200,10,600,400);
    gr9->Draw("AC*");
    gr9->Print(Ssigmagraph.str().c_str());


	myroot.Close();


  return;
}
