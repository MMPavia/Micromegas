#include <stdio.h>
#include <string>
#include <vector>

#define inpath ""

void Macro_frame(string scan1, string scan2="", string scan3="", string title= ""){

    
    double minbin = 25;
    double maxbin = 27;

    ostringstream rootfile;
    rootfile  << title <<".root";
    TFile* myroot = new TFile(rootfile.str().c_str(), "RECREATE");
    
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
//    ostringstream file4;
//    file4 << inpath  << scan4 ;
//    ostringstream file5;
//    file5 << inpath  << scan5 ;
//    ostringstream file6;
//    file6 << inpath  << scan6 ;
    
    ifstream in1(file1.str().c_str());
   	ifstream in2(file2.str().c_str());
    ifstream in3(file3.str().c_str());
//    ifstream in4(file4.str().c_str());
//   	ifstream in5(file5.str().c_str());
//    ifstream in6(file6.str().c_str());
    
    std::vector < std::vector<std::vector <double > >> pos_X_ALL_UP;
    std::vector < std::vector<std::vector <double > >> pos_Y_ALL_UP;
    std::vector < std::vector<std::vector <double > >> pos_opt_ALL_UP;
    std::vector < std::vector<std::vector <double > >> pos_tast_ALL_UP;
    
    std::vector < std::vector<std::vector <double > >> pos_X_ALL_DOWN;
    std::vector < std::vector<std::vector <double > >> pos_Y_ALL_DOWN;
    std::vector < std::vector<std::vector <double > >> pos_opt_ALL_DOWN;
    std::vector < std::vector<std::vector <double > >> pos_tast_ALL_DOWN;
    
    std::vector < std::vector<std::vector <double > >> pos_X_ALL_REFERENCE;
    std::vector < std::vector<std::vector <double > >> pos_Y_ALL_REFERENCE;
    std::vector < std::vector<std::vector <double > >> pos_opt_ALL_REFERENCE;
    std::vector < std::vector<std::vector <double > >> pos_tast_ALL_REFERENCE;
    
    
    
    
    
    // UP FILE
    
    {
    
        
        std::vector<std::vector <double > > pos_X_single_frame_UP;
        std::vector<std::vector <double > > pos_Y_single_frame_UP;
        std::vector<std::vector <double > > pos_tast_single_frame_UP;
        std::vector<std::vector <double > > pos_opt_single_frame_UP;
        
        
        std::vector <double> temp1;
        std::vector <double> temp2;
        std::vector <double> temp3;
        std::vector <double> temp4;
        bool cond = true;
        
        while(cond) {
            

            double pos_X, pos_Y, tast_val, opt_val, date, time;
            
     

        
            in1 >> pos_X >> pos_Y  >> opt_val >> tast_val >> date >> time;

            
            if ( !in1.good() || in1.eof()) cond=false;

            
            if (( temp2.size() == 0 || pos_Y==temp2.back()) && !in1.eof()){
                
            
                temp1.push_back(pos_X);
                temp2.push_back(pos_Y);
                temp3.push_back(tast_val);
                temp4.push_back(opt_val);

            
            }
            
            else
                
            {
             

                
                if( pos_X_single_frame_UP.size()==0 ){
                

                    
                    pos_X_single_frame_UP.push_back(temp1);
                    pos_Y_single_frame_UP.push_back(temp2);
                    pos_tast_single_frame_UP.push_back(temp3);
                    pos_opt_single_frame_UP.push_back(temp4);
                    
                    temp1.clear();
                    temp2.clear();
                    temp3.clear();
                    temp4.clear();
                    
                    
                    
                    temp1.push_back(pos_X);
                    temp2.push_back(pos_Y);
                    temp3.push_back(tast_val);
                    temp4.push_back(opt_val);
                    
                    
                    if(pos_X_single_frame_UP.size()==1){
                    
                        if (pos_X_ALL_UP.size()% 2!= 0){
                            std::reverse(pos_X_single_frame_UP[0].begin(), pos_X_single_frame_UP[0].end() );
                            std::reverse(pos_Y_single_frame_UP[0].begin(), pos_Y_single_frame_UP[0].end() );
                            std::reverse(pos_opt_single_frame_UP[0].begin(), pos_opt_single_frame_UP[0].end() );
                            std::reverse(pos_tast_single_frame_UP[0].begin(), pos_tast_single_frame_UP[0].end() );
                            
                        }
                        
                        
                        pos_X_ALL_UP.push_back(pos_X_single_frame_UP);
                        pos_Y_ALL_UP.push_back(pos_Y_single_frame_UP);
                        pos_opt_ALL_UP.push_back(pos_opt_single_frame_UP);
                        pos_tast_ALL_UP.push_back(pos_tast_single_frame_UP);
                        
                        pos_X_single_frame_UP.clear();
                        pos_Y_single_frame_UP.clear();
                        pos_tast_single_frame_UP.clear();
                        pos_opt_single_frame_UP.clear();
                    
                    }
                
                }
                
                
                else {
                
                    //closing?
                    
                   // std::cout << tast_val << "close" << std::endl;

 
                    
   
                
                }
            
            
            }
        
        
        }
    }
    
    //DOWN
    
    {
        
        
        std::vector<std::vector <double > > pos_X_single_frame_DOWN;
        std::vector<std::vector <double > > pos_Y_single_frame_DOWN;
        std::vector<std::vector <double > > pos_tast_single_frame_DOWN;
        std::vector<std::vector <double > > pos_opt_single_frame_DOWN;
        
        
        std::vector <double> temp1;
        std::vector <double> temp2;
        std::vector <double> temp3;
        std::vector <double> temp4;
        
        bool cond = true;
        
        while(cond) {
            
            
            double pos_X, pos_Y, tast_val, opt_val, date, time;
            
            
            
            
            in2 >> pos_X >> pos_Y  >> opt_val >> tast_val >> date >> time;
            
            if ( !in2.good() || in2.eof()) cond=false;

            
            if ( (temp2.size() == 0 || pos_Y==temp2.back()) && !in2.eof() ){
                
                
                temp1.push_back(pos_X);
                temp2.push_back(pos_Y);
                temp3.push_back(tast_val);
                temp4.push_back(opt_val);
                
                //                std::cout << tast_val << std::endl;
                
            }
            
            else
                
            {
                
                
                
                if( pos_X_single_frame_DOWN.size()==0 ){
                    
                    
                    
                    pos_X_single_frame_DOWN.push_back(temp1);
                    pos_Y_single_frame_DOWN.push_back(temp2);
                    pos_tast_single_frame_DOWN.push_back(temp3);
                    pos_opt_single_frame_DOWN.push_back(temp4);
                    
                    temp1.clear();
                    temp2.clear();
                    temp3.clear();
                    temp4.clear();
                    
                    
                    
                    temp1.push_back(pos_X);
                    temp2.push_back(pos_Y);
                    temp3.push_back(tast_val);
                    temp4.push_back(opt_val);
                    
                    
                    
                    if(pos_X_single_frame_DOWN.size()==1){
                        
                        if (pos_X_ALL_UP.size()% 2!= 0){
                            std::reverse(pos_X_single_frame_DOWN[0].begin(), pos_X_single_frame_DOWN[0].end() );
                            std::reverse(pos_Y_single_frame_DOWN[0].begin(), pos_Y_single_frame_DOWN[0].end() );
                            std::reverse(pos_opt_single_frame_DOWN[0].begin(), pos_opt_single_frame_DOWN[0].end() );
                            std::reverse(pos_tast_single_frame_DOWN[0].begin(), pos_tast_single_frame_DOWN[0].end() );
                            
                        }
                        
                        pos_X_ALL_DOWN.push_back(pos_X_single_frame_DOWN);
                        pos_Y_ALL_DOWN.push_back(pos_Y_single_frame_DOWN);
                        pos_opt_ALL_DOWN.push_back(pos_opt_single_frame_DOWN);
                        pos_tast_ALL_DOWN.push_back(pos_tast_single_frame_DOWN);
                        
                        pos_X_single_frame_DOWN.clear();
                        pos_Y_single_frame_DOWN.clear();
                        pos_tast_single_frame_DOWN.clear();
                        pos_opt_single_frame_DOWN.clear();
                        
                    }
                    
                }
                
                
                else {
                    
                    //closing?
                    
                    // std::cout << tast_val << "close" << std::endl;
                    
                    
                    
                    
                    
                }
                
                
            }
            
            
        }
    }
    

    //REFERENCE
    
    {
        
        
        std::vector<std::vector <double > > pos_X_single_frame_REFERENCE;
        std::vector<std::vector <double > > pos_Y_single_frame_REFERENCE;
        std::vector<std::vector <double > > pos_tast_single_frame_REFERENCE;
        std::vector<std::vector <double > > pos_opt_single_frame_REFERENCE;
        
        
        std::vector <double> temp1;
        std::vector <double> temp2;
        std::vector <double> temp3;
        std::vector <double> temp4;
        
        bool cond=true;
        
        while(cond) {
            
            
            double pos_X, pos_Y, tast_val, opt_val, date, time;
            
            in3 >> pos_X >> pos_Y  >> opt_val >> tast_val >> date >> time;

            
            if ( !in3.good() || in3.eof()) cond=false;
            
            
            
            
            
            
            if ( (temp2.size() == 0 || pos_Y==temp2.back()) &&  !in3.eof()  ){
                
                
                temp1.push_back(pos_X);
                temp2.push_back(pos_Y);
                temp3.push_back(tast_val);
                temp4.push_back(opt_val);
                
                
            }
            
            else
                
            {
                
                
                
                if(  pos_X_single_frame_REFERENCE.size()==0 ){
                    
                    
                    
                    pos_X_single_frame_REFERENCE.push_back(temp1);
                    pos_Y_single_frame_REFERENCE.push_back(temp2);
                    pos_tast_single_frame_REFERENCE.push_back(temp3);
                    pos_opt_single_frame_REFERENCE.push_back(temp4);
                    
                    temp1.clear();
                    temp2.clear();
                    temp3.clear();
                    temp4.clear();
                    
                    
                    
                    temp1.push_back(pos_X);
                    temp2.push_back(pos_Y);
                    temp3.push_back(tast_val);
                    temp4.push_back(opt_val);
                    
                    
                    
                    
                    if(pos_X_single_frame_REFERENCE.size()==1){
                        
                        if (pos_X_ALL_REFERENCE.size()% 2!= 0){
                            std::reverse(pos_X_single_frame_REFERENCE[0].begin(), pos_X_single_frame_REFERENCE[0].end() );
                            std::reverse(pos_Y_single_frame_REFERENCE[0].begin(), pos_Y_single_frame_REFERENCE[0].end() );
                            std::reverse(pos_opt_single_frame_REFERENCE[0].begin(), pos_opt_single_frame_REFERENCE[0].end() );
                            std::reverse(pos_tast_single_frame_REFERENCE[0].begin(), pos_tast_single_frame_REFERENCE[0].end() );
                            
                        }
                        
                        
                        pos_X_ALL_REFERENCE.push_back(pos_X_single_frame_REFERENCE);
                        pos_Y_ALL_REFERENCE.push_back(pos_Y_single_frame_REFERENCE);
                        pos_opt_ALL_REFERENCE.push_back(pos_opt_single_frame_REFERENCE);
                        pos_tast_ALL_REFERENCE.push_back(pos_tast_single_frame_REFERENCE);
                        
                        pos_X_single_frame_REFERENCE.clear();
                        pos_Y_single_frame_REFERENCE.clear();
                        pos_tast_single_frame_REFERENCE.clear();
                        pos_opt_single_frame_REFERENCE.clear();
                        
                    }
                    
                }
                
                
                else {
                    
                    //closing?
                    
                    // std::cout << tast_val << "close" << std::endl;
                    
                    
                    
                    
                    
                }
                
                
            }
            
            
        }
    }
    
    // -------------------------- LET's CALCULATE!!!!!!-------------- //
    
    
    TH1F *frame4 = new TH1F("ThicknessUPAverageALL","ThicknessUPAverageALL", 200 , minbin, maxbin );
    TH1F *frame7 = new TH1F("ThicknessDOWNAverageALL","ThicknessDOWNAverageALL", 200 , minbin, maxbin );
    TH1F *frame5 = new TH1F("BendingALL","BendingALL", 200 , -1, 1 );
    TH1F *frame8 = new TH1F("Thickness","Thickness", 200 , minbin, maxbin );
    
    for( int frame = 0; frame <pos_X_ALL_UP.size(); frame++){
    
 
        
        
//        {
//        std::cout << "Frame " << frame << "\t" << std::endl;
//
//        std::cout << "passata1" << std::endl;
//        
//        for (int k=0; k<pos_X_ALL_UP[frame][0].size(); k++) {
//            
//            std::cout << pos_tast_ALL_UP[frame][0][k] << std::endl;
//        }
//        std::cout << "passata2" << std::endl;
//
//        for (int k=0; k<pos_X_ALL_UP[frame][1].size(); k++) {
//            
//            std::cout << pos_tast_ALL_UP[frame][1][k] << std::endl;
//        }
//        
//        }
        
        

    
    
        std::stringstream ss;
        ss << frame;
        
        
        TDirectory *cdtof = myroot->mkdir(ss.str().c_str());
        cdtof->cd();
        
        

        
        
        
        std::vector <double > mean_reference;
        
        
        for (int point=0; point<pos_X_ALL_REFERENCE[frame][0].size() ; point++) {
            
            
            double alt0_ref = abs(pos_opt_ALL_REFERENCE[frame][0][point]-pos_tast_ALL_REFERENCE[frame][0][point]);
            
            
            
            
            mean_reference.push_back((alt0_ref));
        }
        

        
        TH2F *tmap1= new TH2F("MAP_UP","MAP_UP", 1, 0, 1, pos_opt_ALL_REFERENCE[frame][0].size(), 0, pos_opt_ALL_REFERENCE[frame][0].size() );
       
        tmap1->SetTitleOffset(2);
        tmap1->GetXaxis()->SetTitle("AU");
//        tmap1->GetZaxis()->SetRangeUser(4.50,5.70);
        tmap1->GetYaxis()->SetTitle("AU");
        tmap1->GetZaxis()->SetTitle("Z (#mum)");
        
        
        TH2F *tmap2= new TH2F("MAP_DOWN","MAP_DOWN", 1, 0, 1, pos_opt_ALL_REFERENCE[frame][0].size(), 0, pos_opt_ALL_REFERENCE[frame][0].size() );
        
        tmap2->SetTitleOffset(2);
        tmap2->GetXaxis()->SetTitle("AU");
//        tmap1->GetZaxis()->SetRangeUser(4.50,5.70);
        tmap2->GetYaxis()->SetTitle("AU");
        tmap2->GetZaxis()->SetTitle("Z (#mum)");
        
        

        
        
        TH1F *frame1 = new TH1F("ThicknessUP","ThicknessUP", 200 , minbin, maxbin );
        TH1F *frame2 = new TH1F("ThicknessDOWN","ThicknessDOWN", 200 , minbin, maxbin );
   	TH1F *frame9 = new TH1F("Thickness","Thickness", 200 , minbin, maxbin );
        
//
        for (int point=0; point<pos_X_ALL_REFERENCE[frame][0].size() ; point++) {
            
            double alt0_ref = abs(pos_opt_ALL_REFERENCE[frame][0][point]-pos_tast_ALL_REFERENCE[frame][0][point]);
            
            
            double alt0_up = abs(pos_opt_ALL_UP[frame][0][point]-pos_tast_ALL_UP[frame][0][point]);
            
            
            double alt0_down = abs(pos_opt_ALL_DOWN[frame][0][point]-pos_tast_ALL_DOWN[frame][0][point]);
            
            
            
            tmap1->Fill(1, point, alt0_up-alt0_ref);
            
//            std::cout << alt0_up-alt0_ref <<"\t" << alt1_up-alt1_ref << std::endl;

            frame1->Fill(alt0_up-alt0_ref);
            
            
            tmap2->Fill(1, point, alt0_down-alt0_ref);
            
            frame2->Fill(alt0_down-alt0_ref);
            
            frame4->Fill(alt0_up-alt0_ref);
	    frame7->Fill((alt0_down-alt0_ref));
            frame8->Fill((alt0_down-alt0_ref)+(alt0_up-alt0_up)/2);
            frame9->Fill((alt0_down-alt0_ref)+(alt0_up-alt0_up)/2);
        }
        

        
        // Calculate bending
        
        std::vector <double> up_0_vec;

        std::vector <double> down_0_vec;

        
        
	  cout<<"pos_opt_ALL_UP[frame][0].size() "<<pos_opt_ALL_UP[frame][0].size()<<endl;
        for (int i=0; i<pos_opt_ALL_UP[frame][0].size();i++ ){
        
            up_0_vec.push_back(abs(pos_opt_ALL_UP[frame][0][i]-pos_tast_ALL_UP[frame][0][i])-abs(pos_opt_ALL_REFERENCE[frame][0][i]-pos_tast_ALL_REFERENCE[frame][0][i]));
        
        
        }
        


        
        
        for (int i=0; i<pos_opt_ALL_DOWN[frame][0].size();i++ ){
            
            down_0_vec.push_back(abs(pos_opt_ALL_DOWN[frame][0][i]-pos_tast_ALL_DOWN[frame][0][i])-abs(pos_opt_ALL_REFERENCE[frame][0][i]-pos_tast_ALL_REFERENCE[frame][0][i]));
            
            
        }
        


        
        double max_0_0 = *std::max_element(up_0_vec.begin(), up_0_vec.end());
        double min_0_0 = *std::min_element(up_0_vec.begin(), up_0_vec.end());
	  cout<<"max_0_0: "<<max_0_0<<" min_0_0: "<<min_0_0<<endl;
	  for (int i=0; i<up_0_vec.size();i++) cout<<"up_0_vec "<<up_0_vec.at(i)<<endl;


        
        double max_1_0 = *std::max_element(down_0_vec.begin(), down_0_vec.end());
        double min_1_0 = *std::min_element(down_0_vec.begin(), down_0_vec.end());

        
        

        
        
        // writing trees
        
        
        
        float a1,a2,b1,b2,c1,c2;
        

        TTree *tr1 = new TTree("bending","bending");
        
        tr1->Branch("Up",&b1,"b1/F");
        tr1->Branch("Down",&b2,"b2/F");
        

        
        b1 = max_0_0-min_0_0;
        b2 = max_1_0 - min_1_0;
        
        frame5->Fill(b1);
        frame5->Fill(b2);
        
        
        tr1->Fill();
        
        
        tr1->Write();
        tmap1->Write();
        tmap2->Write();

        
        frame1->Write();
        frame2->Write();
        frame9->Write();
        cdtof->cd("..");

    } //each frame
    
    frame4->Write();
    frame5->Write();
    frame7->Write();
    frame8->Write();

myroot->Close();
}
