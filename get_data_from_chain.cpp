#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <map>

#include "TString.h"
#include "TFile.h"
#include "TF1.h"
#include "TTree.h"
#include "TRandom2.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TMath.h"
#include "THStack.h"
#include "TStyle.h"
#include "TObject.h"
#include "TGraph.h"
#include "TGraph2D.h"

using namespace std;
const int NoEle = 12;
const int NoDet = 7;
const char* DetNames[NoDet] = {"DetReBef", "coll1Bef", "coll1Aft", "coll2Bef", "coll2Aft", "DetT1Bef", "DetT1Aft"};
//const char* DetNames[NoDet] = {"coll1Bef", "coll1Aft", "DetReBef", "coll2Bef", "coll2Aft", "coll3Bef", "coll3Aft", "DetEnd"};
//const char* DetNames[NoDet] = {"coll1Bef", "coll1Aft", "DetReBef", "coll2Bef", "coll2Aft", "coll3Bef", "DetEnd"};

string dir1 = "50_10withDeg";

int scanFiles(vector<string> &fileList, string dir2)
{

    DIR *p_dir;

    p_dir = opendir(dir2.c_str());   // 打开str目录
    if( p_dir == NULL)
    {
        cout<< "can't open" << endl;
    }

    struct dirent *p_dirent;

    while ( p_dirent = readdir(p_dir))
    {
        string tmpFileName = p_dirent->d_name;
        if( tmpFileName == "." || tmpFileName == "..")
        {
            continue;
        }
        else
        {
            fileList.push_back(tmpFileName);
        }
    }
    closedir(p_dir);

    cout << fileList.size() << endl;
    
    return fileList.size();
}


int main()
{
    std::cout << "Type the directory name." << std::endl;
    std::cin >> dir1;
    const string dir2 = "./" + dir1;

    TFile *myFile = new TFile((dir1 + ".root").c_str(),"RECREATE");

    Float_t x, y, z, xx, yy, Px, Py, Pz, t, PDGid, EventID, TrackID, ParentID, Weight, energy;
    Float_t Element[NoEle] = {x, y, z, Px, Py, Pz, t, PDGid, EventID, TrackID, ParentID, Weight};

    const char* Elements[NoEle] = {"x", "y", "z", "Px", "Py", "Pz", "t", "PDGid", "EventID", "TrackID", "ParentID", "Weight"};

    vector<string> fileList;

    scanFiles(fileList, dir2);

    TChain** chN = new TChain*[NoDet];
    
    for(int j = 0; j < NoDet; j++)
    {   
        const string first1 = "./";
        const string houzhui = ".txt";
        fstream file;
        file.open((first1 + DetNames[j] + houzhui).c_str(), ios::out);

        int numOf150 = 0;
        int numOf160 = 0;
        int numOf170 = 0;
        int numOf180 = 0;
        int numOf190 = 0;
        int numOf200 = 0;
        chN[j] = new TChain(DetNames[j]);
       
        for(int i = 0; i < fileList.size(); i++)
        {
            chN[j]->Add((dir2 + "/" + fileList[i] + "/VirtualDetector/" + DetNames[j]).c_str());
        }

        for(int i = 0; i < NoEle; i++)
        {
            chN[j]->SetBranchAddress(Elements[i], &(Element[i]));
        }

        Int_t entries = chN[j]->GetEntries();
        cout << DetNames[j] <<  " 粒子数有： " <<  entries << endl;
        
        for(int k = 0; k < entries; k++)
        {
            chN[j]->GetEntry(k);
            energy = TMath::Sqrt(938.27231*938.27231+Element[3]*Element[3]+Element[4]*Element[4]+Element[5]*Element[5])-938.27231;
            x = Element[0];
            y = Element[1]-512.106;
            xx = 1000*Element[3]/Element[5];
            yy = 1000*Element[4]/Element[5];

            if(TMath::Abs(x)<100 && TMath::Abs(xx)<500 && TMath::Abs(y)<100 && TMath::Abs(yy)<500) {file << x << " " << xx << " " << y << " " << yy << " " << energy << std::endl;}
        }
        file.close();
        std:: cout << DetNames[j] + houzhui << " has been written." << std::endl;
        
    }
    delete[] chN;
    return 0;
}