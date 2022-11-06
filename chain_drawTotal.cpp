#include <iostream>
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
const int NoBefDet = 32;
std::vector<string> DetNames = {"DetReBef", "coll1Bef", "coll1Aft", "coll2Bef", "coll2Aft", "DetT1Bef", "DetT1Aft"};
//const char* DetNames[NoDet] = {"DetReBef", "DetReAft"};
//const char* DetNames[NoDet] = {"coll1Bef", "coll1Aft", "DetReBef", "coll2Bef", "coll2Aft", "coll3Bef", "coll3Aft", "DetEnd"};
//const char* DetNames[NoDet] = {"coll1Bef", "coll1Aft", "DetReBef", "coll2Bef", "coll2Aft", "coll3Bef", "DetEnd"};

string dir1 = "800MeV_150mm20221026";

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


void DrawOneDim(TH1F *xDistr, TH1F *yDistr, string name, TFile* myFile)
{

    Double_t w = 700;
    Double_t h = 700;
    TCanvas *c1 = new TCanvas("c1", "", w, h);
    c1->SetWindowSize(w + (w - c1->GetWw()), h + (h - c1->GetWh()));

    //TPad *pad1 = new TPad("pad1","pad1",0.03,0.62,0.50,0.92,32);
    //pad1->Draw();

    //pad1->cd();
    //pad1->SetLogy();
    //pad1->SetGridx();
    //pad1->SetGridy();
    c1->SetLogy();
    c1->SetGridx();
    c1->SetGridy();

    c1->SetLeftMargin(0.2);
    c1->SetBottomMargin(0.2);

    THStack *hstack = new THStack("hstack", "X and Y distribution of protons; Length (mm); Entries");
    xDistr->SetLineColor(kBlue);
    yDistr->SetLineColor(kRed);
    //xDistr->Scale(1/xDistr->GetEntries());
    //yDistr->Scale(1/yDistr->GetEntries());

    hstack->Add(xDistr);
    hstack->Add(yDistr);

    TF1 *fit = new TF1("fit", "gaus");

    fit->SetLineWidth(3); // 设置函数线条的粗细
    fit->SetLineColor(kRed); // 线条颜色
    fit->SetLineStyle(1); // 线条为虚线

    //fit->SetParameter(0, 1E5); // 最高值
    //fit->SetParameter(1, -16); // mean
    //fit->SetParameter(2, 10); // sigma

    //hist->Fit("fit");

    TLegend *leg = new TLegend(0.8, 0.8, 0.9, 0.9); // 整张画布是从0到1。左下角的x和y坐标，右上角的x和y坐标
    leg->AddEntry(xDistr, "X   ", "l");
    leg->AddEntry(yDistr, "Y   ", "l"); // 图例为直线
    //gStyle->SetLegendFont(40);

    hstack->Draw("nostack");
    hstack->GetXaxis()->CenterTitle(true);
    hstack->GetYaxis()->CenterTitle(true);
    hstack->GetXaxis()->SetTitleSize(0.05); // 坐标轴标题的大小
    hstack->GetYaxis()->SetTitleSize(0.05);
    hstack->GetXaxis()->SetLabelSize(0.05); // 坐标轴刻度值的大小
    hstack->GetYaxis()->SetLabelSize(0.05);

    leg->Draw();

    string houzhui1 = "XplusY.png";
    string houzhui2 = "XplusY.root";

    //c1->Print((name + houzhui1).c_str());
    //myFile->cd();
    myFile->WriteObject(c1, (string(name) + "XplusY").c_str());

    delete c1;
}

void DrawTwoDim(TH2F *hist2D, string name, string type, TFile* myFile)
{
    Double_t w = 700;
    Double_t h = 700;
    TCanvas *c2 = new TCanvas("c2", "", w, h);
    c2->SetWindowSize(w + (w - c2->GetWw()), h + (h - c2->GetWh()));

    c2->SetGridx();
    c2->SetGridy();

    //hist2D->Scale(1/hist2D->GetEntries());

    //c2->SetLogz();
    
    c2->SetRightMargin(0.2);
    c2->SetLeftMargin(0.2);
    c2->SetBottomMargin(0.2);

    gStyle->SetPalette(kRainBow);
    
    hist2D->SetStats(0);

    hist2D->SetContour(100);
    hist2D->Draw("colz");
    hist2D->GetXaxis()->CenterTitle(true);
    hist2D->GetYaxis()->CenterTitle(true);
    hist2D->GetZaxis()->CenterTitle(true);

    hist2D->GetXaxis()->SetTitleSize(0.05); // 坐标轴标题的大小
    hist2D->GetYaxis()->SetTitleSize(0.05);
    hist2D->GetZaxis()->SetTitleSize(0.05);
    hist2D->GetXaxis()->SetLabelSize(0.05); // 坐标轴刻度值的大小
    hist2D->GetYaxis()->SetLabelSize(0.05);

    string houzhui1 = ".png";
    //c2->Print((name + type + houzhui1).c_str());
    //myFile->cd();
    myFile->WriteObject(c2, (name + type).c_str());
    delete c2;
}

void DrawThreeDim(TH3F *hist3D, string name, string type, TFile* myFile)
{
    Double_t w = 700;
    Double_t h = 700;
    TCanvas *c3 = new TCanvas("c3", "", w, h);
    c3->SetWindowSize(w + (w - c3->GetWw()), h + (h - c3->GetWh()));

    c3->SetGridx();
    c3->SetGridy();

    //c3->SetLogz();
    
    c3->SetRightMargin(0.2);
    c3->SetLeftMargin(0.2);
    c3->SetBottomMargin(0.2);
    
    gStyle->SetPalette(kRainBow);

    hist3D->SetContour(1000);
    hist3D->Draw("colz");
    hist3D->GetXaxis()->CenterTitle(true);
    hist3D->GetYaxis()->CenterTitle(true);
    hist3D->GetZaxis()->CenterTitle(true);
    hist3D->GetXaxis()->SetTitleSize(0.05); // 坐标轴标题的大小
    hist3D->GetYaxis()->SetTitleSize(0.05);
    hist3D->GetZaxis()->SetTitleSize(0.05);
    hist3D->GetXaxis()->SetLabelSize(0.05); // 坐标轴刻度值的大小
    hist3D->GetYaxis()->SetLabelSize(0.05);
    
    string houzhui1 = ".png";
    c3->Print((name + type + houzhui1).c_str());
    //myFile->cd();
    myFile->WriteObject(c3, (name + type).c_str());
    delete c3;
}


int main()
{

    std::cout << "Type the directory name." << std::endl;
    std::cin >> dir1;
    const string dir2 = "./" + dir1;

    TFile *myFile = new TFile((dir1 + "total.root").c_str(),"RECREATE");

    Float_t x, y, z, xx, yy, Px, Py, Pz, t, PDGid, EventID, TrackID, ParentID, Weight, energy;
    Float_t Element[NoEle] = {x, y, z, Px, Py, Pz, t, PDGid, EventID, TrackID, ParentID, Weight};

    const char* Elements[NoEle] = {"x", "y", "z", "Px", "Py", "Pz", "t", "PDGid", "EventID", "TrackID", "ParentID", "Weight"};

    vector<string> fileList;

    scanFiles(fileList, dir2);

    for(int i = 1; i < NoBefDet+1; i++)
    {
        //const char* tmp = ("Det" + std::to_string(i)).c_str();
        string tmp = "Det" + std::to_string(i);
        DetNames.push_back(tmp);
    }

    TChain** chN = new TChain*[NoDet+NoBefDet];
    
    for(int j = 0; j < NoDet+NoBefDet; j++)
    {   
        std::cout << j  << ": " << DetNames[j] << std::endl;
        int numOf150 = 0;
        int numOf160 = 0;
        int numOf170 = 0;
        int numOf180 = 0;
        int numOf190 = 0;
        int numOf200 = 0;
        chN[j] = new TChain(DetNames[j].c_str());
       
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

        TH1F *xDistr = new TH1F("xDistr", "X distribution", 240, -120, 120);
        TH1F *yDistr = new TH1F("yDistr", "Y distribution", 200, 0, 700);
        //TH1F *energyDistr = new TH1F("hist", "Energy distribution after degrader", 1600, 0, 1600);
        //TH2F *x_PPx = new TH2F("x_PPx", "X-X' distribution; X (mm); X' (mrad); Entries", 240, -120, 120, 400, -300, 300);
        //TH2F *y_PPy = new TH2F("y_PPy", "Y-Y' distribution; Y (mm); Y' (mrad); Entries", 240, -120, 120, 400, -300, 300);
        TH2F *x_y = new TH2F("x_y", "X-Y distribution; X (mm); Y (mm); Entries", 120, -120, 120, 100, 0, 700);
        //TH3F *x_PPx_e = new TH3F("x_PPx_e", "X-X'-energy distribution; X (mm); X' (mrad); Energy (MeV)", 240, -120, 120, 400, -1000, 1000, 1600, 0, 1600);
        //TH3F *y_PPy_e = new TH3F("y_PPy_e", "Y-Y'-energy distribution; Y (mm); Y' (mrad); Energy (MeV)", 240, -120, 120, 400, -1000, 1000, 1600, 0, 1600);
        
        for(int k = 0; k < entries; k++)
        {
            chN[j]->GetEntry(k);
            energy = TMath::Sqrt(938.27231*938.27231+Element[3]*Element[3]+Element[4]*Element[4]+Element[5]*Element[5])-938.27231;
            x = Element[0];
            y = Element[1];
            xx = 1000*Element[3]/Element[5];
            yy = 1000*Element[4]/Element[5];

            //xDistr->Fill(x);
            //yDistr->Fill(y);
            //x_PPx->Fill(x, xx);
            //y_PPy->Fill(y, yy);
            x_y->Fill(x, y);
            //x_PPx_e->Fill(x, xx, energy);
            //y_PPy_e->Fill(y, yy, energy);
            
        }

        //DrawOneDim(xDistr, yDistr, DetNames[j], myFile);
        DrawTwoDim(x_y, DetNames[j], "x_y", myFile);
        //DrawTwoDim(x_PPx, DetNames[j], "x_PPx", myFile);
        //DrawTwoDim(y_PPy, DetNames[j], "y_PPy", myFile);
        //DrawThreeDim(x_PPx_e, DetNames[j], "x_PPx_e");
        //DrawThreeDim(y_PPy_e, DetNames[j], "y_PPy_e");

        delete xDistr;
        delete yDistr;
        //delete energyDistr;
        //delete x_PPx;
        //delete y_PPy;
        delete x_y;
        //delete x_PPx_e;
        //delete y_PPy_e;
        
    }
    myFile->Close();
    delete[] chN;
    return 0;
}