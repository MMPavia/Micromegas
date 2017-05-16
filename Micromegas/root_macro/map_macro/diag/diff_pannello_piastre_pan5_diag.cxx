#include <stdio.h>
#include <string>
#include <vector>


#define inpath "/Users/mariacristinalopresti/Desktop/mm/gauge-laser/pan5/diagonali/"
#define outpath "/Users/mariacristinalopresti/Desktop/mm/gauge-laser/pan5/diagonali/"

//p=1 laser int
//p=2 laser ext
//p=3 gauge int
//p=4 gauge ext
//string scan = run sul pannello 


void diff_pannello_piastre_pan5_diag (int p, string scan){

  // style option
  //gROOT->LoadMacro("AtlasUtils.C");   // myText
   //SetAtlasStyle(); 
   gStyle->SetPalette(1);
   gStyle->SetOptStat(111111);
   gStyle->SetPaintTextFormat(".3f"); 
   gStyle->SetOptTitle(0);
   gStyle->SetPadRightMargin(0.15);    // serve per mettere margine per non tagliare asse Z
   gStyle->SetNumberContours(100);


// apro il file
   ostringstream infile1;
   if(p==1){
        infile1 << inpath << "mean_and_rep_diag_laser_int_sx_ref.root";
   }
   else if(p==2){
        infile1 << inpath << "mean_and_rep_diag_laser_ext_sx_ref.root";
   }
   else if(p==3){
        infile1 << inpath << "mean_and_rep_diag_gauge_int_sx_ref.root";
   }
   else if(p==4){
        infile1 << inpath << "mean_and_rep_diag_gauge_ext_sx_ref.root";
   }
   ostringstream infile2;
   infile2 << outpath  << scan ;

   scan.erase(scan.end()-5,scan.end());

   TFile *s1 = new TFile(infile1.str().c_str());
   TH2F *map1 = (TH2F*)s1->Get("mean_map"); 
   TH2F *ave = (TH2F*)map1->Clone("average");
   ave->SetTitle("Average Map");
   ave->Scale(0.001);

   TFile *s2 = new TFile(infile2.str().c_str());
   TH2F *map2 = (TH2F*)s2->Get("mean_map");
   TH2F *avep = (TH2F*)map2->Clone("average_pan");
   avep->SetTitle("Average Panel Map");
   avep->Scale(0.001);

   Int_t binx = ave->GetNbinsX();
   Int_t biny = ave->GetNbinsY();
   cout << "binx=" << binx << "biny " << biny << endl;
   

    Float_t xmin,xmax,ymin,ymax;
if(p==1 || p==3){             //diag interne
        xmin = 320/10;   
        xmax = 695/10;
        ymax = 225;
        ymin = 154;
           }
   else if(p==2 || p==4){
        xmin = 331/10;      //diag esterne
        xmax = 707/10;
        ymax = 149;
        ymin = 81;
           }

   float xstep = (xmax-xmin)/binx-1;
   float ystep = (ymax-ymin)/biny-1;


   TH2F *Map_diff= new TH2F("Map_diff","Map_diff", binx, xmin, xmax, biny, ymin, ymax );
   Map_diff->GetXaxis()->SetTitle("X (cm)");
   Map_diff->GetYaxis()->SetTitle("Y (mm)");
   Map_diff->GetZaxis()->SetTitle("Z (#mum)");

   TH1F *z_distrib = new TH1F("Z Distribution","z_distrib", 100, 11.400, 12.000);
   z_distrib->GetXaxis()->SetTitle("Z (mm)");
   z_distrib->SetFillColor(kAzure-5);

   Map_diff->Add(ave,avep,-1,+1); //diffrenza pannello reference 

   float zref,zmap, z, max_z, min_z;
   max_z = -100000;
   min_z = 100000;

   cout << "zref  zmap  zdiff "<< endl;
   //std::ofstream file("z.txt"); 
   //for (int w=0; w<222; ++w )  {

   for (int i=1; i<=biny; i++){
            for (int j=1; j<=binx; j++){
            
            zref= ave->GetBinContent(j,i);
            zmap = avep->GetBinContent(j,i);
            z = Map_diff->GetBinContent(j,i);
            if(z!=0) {
	        z_distrib->Fill(z); 
	        //file << z << std::endl;                //stampare bin x e bin y

            cout << zref << " " << zmap << " " << z << endl;
            cout << z << endl;

	     }
	
         if(z!=0 && z<min_z){min_z = z;}
         if(z!=0 && z>max_z){max_z = z;}
      }
   }
  // }
   //file.close();
   

   diff_ave = (TH2F*)Map_diff->Clone("diff_ave");
   diff_ave->Divide(Map_diff); 
   Float_t averageZ  = z_distrib->GetMean();
   cout <<  "average : " << averageZ << endl; 
   diff_ave->Scale(averageZ);
   diff_ave->Add(Map_diff,diff_ave,+1,-1);
   diff_ave->Scale(1000);

   cout << "*******************" << endl;
   cout << "max_z: "  << max_z << endl;
   cout << "min_z: "  << min_z << endl;
   cout << "mean: "  << averageZ << endl;
   cout << "*******************" << endl;


// create root file

   ostringstream rootfile;
   rootfile  << outpath << "diff_pannello_piastra.root";
   TFile myroot(rootfile.str().c_str(), "RECREATE");
   cout << "Root output file: " << endl << rootfile.str() << endl;

   ostringstream Smap;
   Smap  << outpath  << "/diff_pan_piastra_"<< scan <<".png";  
   ostringstream Smapdiff;
   Smapdiff  << outpath  << "/diff_pan_piastra_"<< scan <<".png";  
   ostringstream Sdistrib;
   Sdistrib  << outpath  << "/diff_pan_piastra_z_"<< scan <<".png"; 



   TCanvas *c = new TCanvas("ref","ref",200, 10, 1000,700); 
   map1->Draw("COLZ1TEXT45");
   map1->SetStats(kFALSE);
   
   TCanvas *cc = new TCanvas("map","map ",200, 10, 1000,700); 
   map2->Draw("COLZ1TEXT45");
   map2->GetZaxis()->SetRangeUser(11.4, 11.8);
   map2->SetStats(kFALSE);
   
   //TCanvas *ccc = new TCanvas(); 
   //dist->Draw();


   TCanvas *c0 = new TCanvas("map corrected", "map corrected", 200, 10, 1000,700);
   Map_diff->Draw("COLZ1TEXT45");
   Map_diff->SetStats(kFALSE); 
   Map_diff->Write();
   Map_diff->GetZaxis()->SetRangeUser(11.4, 11.8);
   c0->Print(Smap.str().c_str());


   TCanvas *c1 = new TCanvas("c1", "c1", 1000,700); 
   z_distrib->Draw(); 
   z_distrib->Write();
   c1->Print(Sdistrib.str().c_str());

   myroot.Close(); 


   return;
}
