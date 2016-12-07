#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Spessore_frame/"
#define outpath "/home/atlas/Micromegas/Simone/Output/Larghezza_frame/Spessore_frame_tutti/"

void Graph_spessore_frame (TString scanI1, TString scanLH1, TString scanM){

	// style option
	
//	gStyle->SetPalette(1);
//	gStyle->SetOptStat(111111);
	gStyle->SetPaintTextFormat(".3f");
	
	// file containing the data
	ostringstream file1;
	file1 << inpath  << scanI1 ;  
	

	//variables for reading the file
	int r = 12;
 	Double_t x[r],a=0,Diff11[r],Diff12[r],Diff13[r],Diff1[r],Diff2[r],Diff3[r], Diff21[r],Diff22[r],Diff23[r],mediaT[r],mediaLH[r],mediaM[r];
   	Double_t  Ione[r],Itwo[r],Ithree[r],u,Lone[r],Ltwo[r],Lthree[r],Mone[r], Mtwo[r], Mthree[r], DiffM1[r], DiffM2[r], DiffM3[r];
   	Int_t nmis;
   	string date, time;

	
	// reading the file
	ifstream in(file1.str().c_str());
	
 	 if (!in || in.bad() )
	 {
		cout << "file non letto" << endl;
		return; // sanity check
	}
	
	cout << "starting reading file " << endl;
        int j=0,i=0,l=0;

	// looping on the file lines
	while (1) {
		
		in >> nmis >> a  >> u  >> u;
		
		if ( !in.good() || in.eof()) break;  // another sanity check

	

        	if (a==0){
       			in >> Ione[j];
       			x[j]= (nmis/10);
			Ione[j]= (Ione[j]+9.995);
			j++;
		}

		if (a==6){
                        in >> Itwo[11-i];
                        x[i]= (nmis/10);
                        Itwo[11-i]= (Itwo[11-i]+9.995);
               		i++;
		 }


		if (a==12){
                        in >> Ithree[l];
                        x[l]= (nmis/10);
                        Ithree[l]= (Ithree[l]+9.995);
			l++;
                }

       		cout <<"x= " << x[j] << endl;
    		cout <<"Ione= " << Ione[j] << endl;
		cout <<"nmis " << nmis << endl;

         
                in >> date >> time; 
                cout << "date " << date <<" time " << time << endl;
				
	} // end while


                cout <<"x= " << x[1] << endl;
                cout <<"Ione[1]= " << Ione[1] << endl;
                cout <<"nmis " << nmis << endl;
                cout <<"x= " << x[j-1] << endl;
                cout <<"Ione[j-1]= " << Ione[j-1] << endl;



	cout << "file completed "<< endl;


	// close the input file
	in.close();





	//Lettura file Linear height

        ostringstream file3;
        file3 << inpath  << scanLH1 ;


	
        ifstream in3(file3.str().c_str());

         if (!in3 || in3.bad() )
         {
                cout << "file non letto" << endl;
                return; // sanity check
        }

        cout << "starting reading file " << endl;
        j=0;

        while (1) {

                in3 >> nmis;

                if ( !in3.good() || in3.eof()) break;  // another sanity check




                in3 >> Lone[j] >> Ltwo[j] >> Lthree[j];

                cout <<"x= " << x[j] << endl;
                cout <<"v= " << Lone[j] << endl;
                cout <<"nmis " << nmis << endl;


                in3 >> date >> time;
                cout << "date " << date <<" time " << time << endl;
                j++;

        }

        in3.close();


	//Lettura file Micrometro
	//

        ostringstream file4;
        file4 << inpath  << scanM ;



        ifstream in4(file4.str().c_str());

         if (!in4 || in4.bad() )
         {
                cout << "file non letto" << endl;
                return; // sanity check
        }

        cout << "starting reading file " << endl;
        j=0;

        while (1) {

                in4 >> nmis;

                if ( !in4.good() || in4.eof()) break;  // another sanity check




                in4 >> Mone[j] >> Mtwo[j] >> Mthree[j];

                cout <<"x= " << x[j] << endl;
                cout <<"v= " << Mone[j] << endl;
                cout <<"nmis " << nmis << endl;


                in4 >> date >> time;
                cout << "date " << date <<" time " << time << endl;
                j++;

        }

        in4.close();




	//Calcolo differenze Tastatore1-LH1, Tastatore2-LH2.
	
	for(j=0;j<12;j++){
	Diff1[j]=(Ione[j]-Lone[j])*1000;
	Diff2[j]=(Itwo[j]-Ltwo[j])*1000;
	Diff3[j]=(Ithree[j] - Lthree[j])*1000;
	mediaT[j]=(Ione[j]+Itwo[j]+Ithree[j])/3;
        mediaLH[j]=(Lone[j]+Ltwo[j]+Lthree[j])/3;
	mediaM[j]=(Mone[j]+Mtwo[j]+Mthree[j])/3;
        Diff11[j]=(Ione[j]-Mone[j])*1000;
        Diff12[j]=(Itwo[j]-Mtwo[j])*1000;
        Diff13[j]=(Ithree[j] - Mthree[j])*1000;
        Diff21[j]=(Lone[j]-Mone[j])*1000;
        Diff22[j]=(Ltwo[j]-Mtwo[j])*1000;
        Diff23[j]=(Lthree[j] - Mthree[j])*1000;
	DiffM1[j]=(mediaT[j] - mediaLH[j])*1000;
	DiffM2[j]=(mediaT[j] - mediaM[j])*1000;
	DiffM3[j]=(mediaLH[j] - mediaM[j])*1000;

	}



        //Creazione Graph
        TGraph *gr1 = new TGraph (12,x,Ione);
        TGraph *gr2 = new TGraph (12,x,Itwo);
        TGraph *gr3 = new TGraph (12,x,Ithree);
        TGraph *gr4 = new TGraph (12,x,Lone);
        TGraph *gr5 = new TGraph (12,x,Ltwo);
        TGraph *gr6 = new TGraph (12,x,Lthree);
        TGraph *gr7 = new TGraph (12,x,Diff1);
	TGraph *gr8 = new TGraph (12,x,Diff2);
        TGraph *gr9 = new TGraph (12,x,Diff3);
        TGraph *gr10 = new TGraph (12,x,mediaT);
        TGraph *gr11 = new TGraph (12,x,mediaLH);
	TGraph *gr21 = new TGraph (12,x,Mone);
        TGraph *gr22 = new TGraph (12,x,Mtwo);
        TGraph *gr23 = new TGraph (12,x,Mthree);
        TGraph *gr24 = new TGraph (12,x,mediaM);
        TGraph *Diff111 = new TGraph (12,x,Diff11);
        TGraph *Diff121 = new TGraph (12,x,Diff12);
        TGraph *Diff131 = new TGraph (12,x,Diff13);
        TGraph *Diff211 = new TGraph (12,x,Diff21);
        TGraph *Diff221 = new TGraph (12,x,Diff22);
        TGraph *Diff231 = new TGraph (12,x,Diff23);
        TGraph *DiffM11 = new TGraph (12,x,DiffM1);
        TGraph *DiffM22 = new TGraph (12,x,DiffM2);
        TGraph *DiffM33 = new TGraph (12,x,DiffM3);

	// create a root file for the histograms; 
	ostringstream rootfile;
	rootfile  << outpath <<"myout_"<<scanI1 <<".root";
	TFile myroot(rootfile.str().c_str(), "RECREATE");
	cout << "Root output file: " << endl << rootfile.str() << endl;


  	 TMultiGraph *mg = new TMultiGraph();



	 mg->Add(gr1); gr1->SetTitle("Tastatore 1");		  gr1->SetLineWidth(3); gr1->SetLineColor(kRed);
  	 mg->Add(gr2); gr2->SetTitle("Tastatore 2");		  gr2->SetLineWidth(3); gr2->SetLineColor(kBlue);
         mg->Add(gr3); gr3->SetTitle("Tastatore 3");         	  gr3->SetLineWidth(3); gr3->SetLineColor(kGreen);
         mg->Add(gr4); gr4->SetTitle("Linear height 1");          gr4->SetLineWidth(3); gr4->SetLineColor(kYellow);
         mg->Add(gr5); gr5->SetTitle("Linear height 2");          gr5->SetLineWidth(3); gr5->SetLineColor(kMagenta);
         mg->Add(gr6); gr6->SetTitle("Linear height 3");          gr6->SetLineWidth(3); gr6->SetLineColor(kBlack);

	TCanvas *c1 = new TCanvas("c1","Graph Draw Options",200,10,600,400);
 	mg->Draw("AC*");


	 leg = new TLegend(0.1,0.7,0.48,0.9);
  	 leg->SetHeader("Spessore frame");
  	 leg->AddEntry(gr1,"Tastatore 1");
         leg->AddEntry(gr2,"Tastatore 2");
         leg->AddEntry(gr3,"Tastatore 3");
         leg->AddEntry(gr4,"Linear height 1");
         leg->AddEntry(gr5,"Linear height 2");
         leg->AddEntry(gr6,"Linear height 3");
  	 leg->Draw();
         c1->Print("/home/atlas/Micromegas/Simone/Output/Larghezza_frame/Spessore_frame_tutti/Andamento_spessore_frame_tast_LH.png");


	//Creazione grafici differenze

         TMultiGraph *diff = new TMultiGraph();
         diff->Add(gr7); gr7->SetTitle("Tastatore 1");              gr7->SetLineWidth(3); gr7->SetLineColor(kRed);
         diff->Add(gr8); gr8->SetTitle("Tastatore 2");              gr8->SetLineWidth(3); gr8->SetLineColor(kBlue);
         diff->Add(gr9); gr9->SetTitle("Tastatore 3");              gr9->SetLineWidth(3); gr9->SetLineColor(kGreen);
         TCanvas *c2 = new TCanvas("c2","Graph Draw Options",200,10,600,400);
         diff->Draw("AC*");
         leg = new TLegend(0.1,0.7,0.48,0.9);
         leg->SetHeader("Differenze spessore frame");
         leg->AddEntry(gr7,"Tastatore1 - LH1");
         leg->AddEntry(gr8,"Tastatore2 - LH2");
         leg->AddEntry(gr9,"Tastatore3 - LH3");
         leg->Draw();
         c2->Print("/home/atlas/Micromegas/Simone/Output/Larghezza_frame/Spessore_frame_tutti/Andamento_differenze_Tastatore_LH.png");
	


         TMultiGraph *diffM = new TMultiGraph();
         diffM->Add(DiffM11); DiffM11->SetTitle("Tastatore 1");              DiffM11->SetLineWidth(3); DiffM11->SetLineColor(kRed);
         diffM->Add(DiffM22); DiffM22->SetTitle("Tastatore 2");              DiffM22->SetLineWidth(3); DiffM22->SetLineColor(kBlue);
         diffM->Add(DiffM33); DiffM33->SetTitle("Tastatore 3");              DiffM33->SetLineWidth(3); DiffM33->SetLineColor(kGreen);
         TCanvas *cmed = new TCanvas("c2","Graph Draw Options",200,10,600,400);
         diffM->Draw("AC*");
         leg = new TLegend(0.1,0.7,0.48,0.9);
         leg->SetHeader("Differenze medie spessore frame");
         leg->AddEntry(DiffM11,"Tastatore - LH");
         leg->AddEntry(DiffM22,"Tastatore - Micrometro");
         leg->AddEntry(DiffM33,"LH - Micrometro");
         leg->Draw();
         cmed->Print("/home/atlas/Micromegas/Simone/Output/Larghezza_frame/Spessore_frame_tutti/Andamento_differenze_medie_3_strumenti.png");



         TMultiGraph *diff2 = new TMultiGraph();
         diff2->Add(Diff111); Diff111->SetTitle("Tastatore 1");              Diff111->SetLineWidth(3); Diff111->SetLineColor(kRed);
         diff2->Add(Diff121); Diff121->SetTitle("Tastatore 2");              Diff121->SetLineWidth(3); Diff121->SetLineColor(kBlue);
         diff2->Add(Diff131); Diff131->SetTitle("Tastatore 3");              Diff131->SetLineWidth(3); Diff131->SetLineColor(kGreen);
         TCanvas *c33 = new TCanvas("c2","Graph Draw Options",200,10,600,400);
         diff2->Draw("AC*");
         leg = new TLegend(0.1,0.7,0.48,0.9);
         leg->SetHeader("Differenze spessore frame");
         leg->AddEntry(Diff111,"Tastatore1 - Micrometro1");
         leg->AddEntry(Diff121,"Tastatore2 - Micrometro2");
         leg->AddEntry(Diff131,"Tastatore3 - Micrometro3");
         leg->Draw();
         c33->Print("/home/atlas/Micromegas/Simone/Output/Larghezza_frame/Spessore_frame_tutti/Andamento_differenze_Tastatore_micrometro.png");


         TMultiGraph *diff3 = new TMultiGraph();
         diff3->Add(Diff211); Diff211->SetTitle("Tastatore 1");              Diff211->SetLineWidth(3); Diff211->SetLineColor(kRed);
         diff3->Add(Diff221); Diff221->SetTitle("Tastatore 2");              Diff221->SetLineWidth(3); Diff221->SetLineColor(kBlue);
         diff3->Add(Diff231); Diff231->SetTitle("Tastatore 3");              Diff231->SetLineWidth(3); Diff231->SetLineColor(kGreen);
         TCanvas *c22 = new TCanvas("c2","Graph Draw Options",200,10,600,400);
         diff2->Draw("AC*");
         leg = new TLegend(0.1,0.7,0.48,0.9);
         leg->SetHeader("Differenze spessore frame");
         leg->AddEntry(Diff211,"LH1 - Micrometro1");
         leg->AddEntry(Diff221,"LH2 - Micrometro2");
         leg->AddEntry(Diff231,"LH3 - Micrometro3");
         leg->Draw();
         c22->Print("/home/atlas/Micromegas/Simone/Output/Larghezza_frame/Spessore_frame_tutti/Andamento_differenze_LH_micrometro.png");




	//Creazione grafici medie
	
         TMultiGraph *medie = new TMultiGraph();
         medie->Add(gr10); gr10->SetTitle("Tastatore");              gr10->SetLineWidth(3); gr10->SetLineColor(kRed);
         medie->Add(gr11); gr11->SetTitle("Linear Height");              gr11->SetLineWidth(3); gr11->SetLineColor(kBlue);
         medie->Add(gr24); gr24->SetTitle("Micrometro");              gr24->SetLineWidth(3); gr24->SetLineColor(kGreen);
         TCanvas *c3 = new TCanvas("c3","Graph Draw Options",200,10,600,400);
         medie->Draw("AC*");
         leg = new TLegend(0.1,0.7,0.48,0.9);
         leg->SetHeader("Larghezza frame media");
         leg->AddEntry(gr10,"Tastatore");
         leg->AddEntry(gr11,"Linear Height");
         leg->AddEntry(gr24,"Micrometro");
         leg->Draw();
	 c3->Print("/home/atlas/Micromegas/Simone/Output/Larghezza_frame/Spessore_frame_tutti/Andamento_larghezza_media_3_strumenti.png");



	
	//3yroot.Close();


  return;
}
