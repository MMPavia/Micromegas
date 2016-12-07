#include <stdio.h>
#include <string>
#include <vector>



#define inpath "/home/atlas/Micromegas/Simone/Input/Frame_3_punti/"
#define outpath "/home/atlas/Micromegas/Simone/Output/"

void ReadLHFrame2 (TString scan){

	// style option
	
	gStyle->SetPalette(1);
	gStyle->SetOptStat(111111);
	gStyle->SetPaintTextFormat(".0f");
	
	// file containing the data
	ostringstream file1;
	file1 << inpath  << scan ;  
	
	// create all the histograms

	TH1F *frame1 = new TH1F("punto1","punto1", 20, 9.90, 10.10 );
        TH1F *frame2 = new TH1F("punto2","punto2", 20, 9.90, 10.10 );
        TH1F *frame3 = new TH1F("punto3","punto3", 20, 9.90, 10.10 );
        TH1F *frame4 = new TH1F("tutti","tutti", 20, 9.90, 10.10 );

	cout << "histos created " << endl;

	//variables for reading the file
	
	Float_t x,y,z;
	Int_t nmis,i=0;
	string date, time;
	Double_t sigma1[12], sigma2[12], sigma3[12];
        Double_t media1=0, media2=0, media3=0;
        Double_t sigma11=0;
        Double_t sigma22=0;
        Double_t sigma33=0;
	
	// reading the file
	ifstream in(file1.str().c_str());
	
 	 if (!in || in.bad() )
	 {
		cout << "file non letto" << endl;
		return; // sanity check
	}
	
	cout << "starting reading file " << endl;

	// looping on the file lines
	while (1) {
		
		in >> nmis;
		
		if ( !in.good() || in.eof()) break;  // another sanity check

		cout <<"nmis " << nmis << endl;
		
		
		
			in >> x ;
			in >> y ;
 			in >> z ;
			cout <<"x" << " " <<  x<< endl;
                        cout <<"y" << " " <<  y<< endl;  
                        cout <<"z" << " " <<  z<< endl;   
			frame1->Fill(x);
			frame2->Fill(y);
			frame3->Fill(z);
			frame4->Fill(x);
                        frame4->Fill(y);
                        frame4->Fill(z);
			x = 0;
                        y = 0;
                        z = 0;

			media1=media1+x;
                        media1=media2+y;
                        media1=media3+z;

			sigma1[i]=x;
			sigma2[i]=y;
                        sigma3[i]=z;		

		in >> date >> time; 
		cout << "date " << date <<" time " << time << endl; 
		
	} // end while
	
	cout << "file completed "<< endl;

	
        media1= media1/12;
        media2= media2/12;
        media3= media3/12;

	for(i=0; i<12;i++){
		sigma1[i]= sigma1[i]-media1;
                sigma2[i]= sigma1[i]-media2;
                sigma3[i]= sigma1[i]-media3;
	}


	for(i=0;i<12;i++)
		sigma11= sigma11 + sigma1[i]*sigma1[i];
                sigma22= sigma22 + sigma2[i]*sigma2[i];
                sigma33= sigma33 + sigma3[i]*sigma3[i];
	}

	
	sigma11= sqrt(sigma11/11);	
        sigma22= sqrt(sigma22/11); 
        sigma33= sqrt(sigma33/11); 


	// close the input file
	in.close();

/*
	// raw values: average and differences wrt to the average
	Float_t value = z_corr->GetMean();
	Float_t sigma = z_corr->GetRMS();
*/
	
	
	// create a root file for the histograms; 
	ostringstream rootfile;
	rootfile  << outpath <<"myout_"<<scan <<".root";
	TFile myroot(rootfile.str().c_str(), "RECREATE");
	cout << "Root output file: " << endl << rootfile.str() << endl;


	// create a canvas for each plot and drawing with different options
	TCanvas *c0 = new TCanvas();
	frame1->Draw();
	frame1->Write();

        TCanvas *c1 = new TCanvas();

	frame2->Draw();
        frame2->Write();

        TCanvas *c2 = new TCanvas();

	frame3->Draw();
        frame3->Write();


        TCanvas *c3 = new TCanvas();

        frame4->Draw();
        frame4->Write();

	
 
	//myroot.Close();


  return;
}
