#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Larghezza_frame/"
#define outpath "/home/atlas/Micromegas/Simone/Output/Larghezza_frame/Largh_frame_tutti/"

void Graph_largh_frame_incollatrice (TString scanI1, TString scanI2, TString scanLH1, TString scanLH2, TString scanm){

	// style option
	
//	gStyle->SetPalette(1);
//	gStyle->SetOptStat(111111);
	gStyle->SetPaintTextFormat(".3f");
	
	// file containing the data
	ostringstream file1;
	file1 << inpath  << scanI1 ;  
	

	//variables for reading the file
	int r = 12;
 	Double_t x[r],Diff1[r],Diff2[r],mediaT[r],mediaLH[r];
   	Double_t  s[r],t[r],u,z[r],q[r],v[r];
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
        int j=0;

	// looping on the file lines
	while (1) {
		
		in >> nmis >> u  >> u  >> u;
		
		if ( !in.good() || in.eof()) break;  // another sanity check

	

        
       		in >> s[j];
       		x[j]= (nmis/10);
		s[j]= (s[j]+26.005);

       		cout <<"x= " << x[j] << endl;
    		cout <<"s= " << s[j] << endl;
		cout <<"nmis " << nmis << endl;

         
                in >> date >> time; 
                cout << "date " << date <<" time " << time << endl;
		j++;
		
	} // end while


                cout <<"x= " << x[1] << endl;
                cout <<"s= " << s[1] << endl;
                cout <<"nmis " << nmis << endl;
                cout <<"x= " << x[j-1] << endl;
                cout <<"s= " << s[j-1] << endl;



	cout << "file completed "<< endl;


	// close the input file
	in.close();


        ostringstream file2;
        file2 << inpath  << scanI2 ;




	//sanity check

        ifstream in1(file2.str().c_str());

         if (!in1 || in1.bad() )
         {
                cout << "file non letto" << endl;
                return; // sanity check
        }

        cout << "starting reading file " << endl;
        j=0;


        while (1) {

                in1 >> nmis >> u  >> u  >> u ;

                if ( !in1.good() || in1.eof()) break;  // another sanity check




                in1 >> t[j];
                t[j]= (t[j]+26.005);

                cout <<"x= " << x[j] << endl;
                cout <<"s= " << t[j] << endl;
                cout <<"nmis " << nmis << endl;


                in1 >> date >> time;
                cout << "date " << date <<" time " << time << endl;
                j++;

        }

        in1.close();


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




                in3 >> v[j];

                cout <<"x= " << x[j] << endl;
                cout <<"v= " << v[j] << endl;
                cout <<"nmis " << nmis << endl;


                in3 >> date >> time;
                cout << "date " << date <<" time " << time << endl;
                j++;

        }

        in3.close();





        ostringstream file4;
        file4 << inpath  << scanLH2 ;



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




                in4 >> z[j];

                cout <<"x= " << x[j] << endl;
                cout <<"z= " << z[j] << endl;
                cout <<"nmis " << nmis << endl;


                in4 >> date >> time;
                cout << "date " << date <<" time " << time << endl;
                j++;

        }

        in4.close();




        ostringstream file5;
        file5 << inpath  << scanm ;



        ifstream in5(file5.str().c_str());
        
         if (!in5 || in5.bad() )
         {
                cout << "file non letto" << endl;
                return; // sanity check
        }

        cout << "starting reading file " << endl;
        j=0;


        while (1) {

                in5 >> nmis;

                if ( !in5.good() || in5.eof()) break;  // another sanity check




                in5 >> q[j];

                cout <<"x= " << x[j] << endl;
                cout <<"q= " << q[j] << endl;
                cout <<"nmis " << nmis << endl;

                j++;

        }

        in5.close();

	//Calcolo differenze Tastatore1-LH1, Tastatore2-LH2.
	
	for(j=0;j<12;j++){
	Diff1[j]=(s[j]-z[j])*1000;
	Diff2[j]=(t[j]-v[j])*1000;
	mediaT[j]=(s[j]+t[j])/2;
        mediaLH[j]=(z[j]+v[j])/2;
	}



        //Creazione Graph
        TGraph *gr1 = new TGraph (12,x,s);
        TGraph *gr2 = new TGraph (12,x,t);
        TGraph *gr4 = new TGraph (12,x,v);
        TGraph *gr3 = new TGraph (12,x,z);
        TGraph *gr5 = new TGraph (12,x,q);
        TGraph *gr6 = new TGraph (12,x,Diff1);
        TGraph *gr7 = new TGraph (12,x,Diff2);
        TGraph *gr8 = new TGraph (12,x,mediaT);
        TGraph *gr9 = new TGraph (12,x,mediaLH);

	// create a root file for the histograms; 
	ostringstream rootfile;
	rootfile  << outpath <<"myout_"<<scanm <<".root";
	TFile myroot(rootfile.str().c_str(), "RECREATE");
	cout << "Root output file: " << endl << rootfile.str() << endl;


  	 TMultiGraph *mg = new TMultiGraph();



	 mg->Add(gr1); gr1->SetTitle("Tastatore 1");		  gr1->SetLineWidth(3); gr1->SetLineColor(kRed);
  	 mg->Add(gr2); gr2->SetTitle("Tastatore 2");		  gr2->SetLineWidth(3); gr2->SetLineColor(kBlue);
         mg->Add(gr3); gr3->SetTitle("Linear height 1");          gr3->SetLineWidth(3); gr3->SetLineColor(kGreen);
         mg->Add(gr4); gr4->SetTitle("Linear height 2");          gr4->SetLineWidth(3); gr4->SetLineColor(kYellow);
         mg->Add(gr5); gr5->SetTitle("Micrometro");         	  gr5->SetLineWidth(3); gr5->SetLineColor(kMagenta);


	TCanvas *c1 = new TCanvas("c1","Graph Draw Options",200,10,600,400);
 	mg->Draw("AC*");


	 leg = new TLegend(0.1,0.7,0.48,0.9);
  	 leg->SetHeader("Larghezza frame");
  	 leg->AddEntry(gr1,"Tastatore 1");
         leg->AddEntry(gr2,"Tastatore 2");
         leg->AddEntry(gr3,"Linear height 1");
         leg->AddEntry(gr4,"Linear height 2");
         leg->AddEntry(gr5,"Micrometro");
  	 leg->Draw();


	//Creazione grafici differenze

         TMultiGraph *diff = new TMultiGraph();
         diff->Add(gr6); gr6->SetTitle("Tastatore 1");              gr6->SetLineWidth(3); gr6->SetLineColor(kRed);
         diff->Add(gr7); gr7->SetTitle("Tastatore 2");              gr7->SetLineWidth(3); gr7->SetLineColor(kBlue);
         TCanvas *c2 = new TCanvas("c2","Graph Draw Options",200,10,600,400);
         diff->Draw("AC*");
         leg = new TLegend(0.1,0.7,0.48,0.9);
         leg->SetHeader("Larghezza frame");
         leg->AddEntry(gr6,"Tastatore1 - LH1");
         leg->AddEntry(gr7,"Tastatore2 - LH2");
         leg->Draw();
	

	//Creazione grafici medie
	
         TMultiGraph *medie = new TMultiGraph();
         medie->Add(gr8); gr8->SetTitle("Tastatore");              gr8->SetLineWidth(3); gr8->SetLineColor(kRed);
         medie->Add(gr9); gr9->SetTitle("Linear Height");              gr9->SetLineWidth(3); gr9->SetLineColor(kBlue);
         medie->Add(gr5); gr5->SetTitle("Linear Height");              gr5->SetLineWidth(3); gr5->SetLineColor(kGreen);
         TCanvas *c3 = new TCanvas("c3","Graph Draw Options",200,10,600,400);
         medie->Draw("AC*");
         leg = new TLegend(0.1,0.7,0.48,0.9);
         leg->SetHeader("Larghezza frame media");
         leg->AddEntry(gr8,"Tastatore");
         leg->AddEntry(gr9,"Linear Height");
         leg->AddEntry(gr5,"Micrometro");
         leg->Draw();




	
	//3yroot.Close();


  return;
}
