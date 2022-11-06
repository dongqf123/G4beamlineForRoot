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
#include "THStack.h"
#include "TROOT.h"

#define gROOT (ROOT::GetROOT())
using namespace std;
const int NoEle = 12;
const int NoDet = 2;
//const char* DetNames[NoDet] = {"DetReBef", "coll1Bef", "coll1Aft", "coll2Bef", "coll2Aft", "DetT1Bef", "DetT1Aft"};
const char* DetNames[NoDet] = {"coll1Bef",  "coll2Bef"};
//const char* DetNames[NoDet] = {"coll1Bef", "coll1Aft", "DetReBef", "coll2Bef", "coll2Aft", "coll3Bef", "coll3Aft", "DetEnd"};
//const char* DetNames[NoDet] = {"coll1Bef", "coll1Aft", "DetReBef", "coll2Bef", "coll2Aft", "coll3Bef", "DetEnd"};

string dir1 = "800MeV_150mm20221026";

TFile *myFile = new TFile((dir1 + ".root").c_str(),"RECREATE");

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


void DrawOneDim(TH1F *xDistr, TH1F *yDistr, const char* name)
{
    gROOT->ForceStyle();
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

    c1->Print((name + houzhui1).c_str());
    //myFile->cd();
    myFile->WriteObject(c1, (string(name) + "XplusY").c_str());
    delete c1;
}

void DrawTwoDim(TH2F *hist2D, const char* name, string type)
{
    gROOT->ForceStyle();
    Double_t w = 700;
    Double_t h = 700;
    TCanvas *c2 = new TCanvas("c2", "", w, h);
    c2->SetWindowSize(w + (w - c2->GetWw()), h + (h - c2->GetWh()));

    c2->SetGridx();
    c2->SetGridy();

    //hist2D->Scale(1/hist2D->GetEntries());

    c2->SetLogz();
    
    c2->SetRightMargin(0.2);
    c2->SetLeftMargin(0.2);
    c2->SetBottomMargin(0.2);

    gStyle->SetPalette(kRainBow);
    
    hist2D->SetStats(0);

    hist2D->SetContour(1000);
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
    c2->Print((name + type + houzhui1).c_str());
    //myFile->cd();
    myFile->WriteObject(c2, (name + type).c_str());
    delete c2;
}

void DrawThreeDim(TGraph2D *gr, const char* name, string type, string title)
{
    
    gROOT->ForceStyle();
    Double_t w = 700;
    Double_t h = 700;
    TCanvas *c3 = new TCanvas("c3", "", w, h);
    c3->SetGridx();
    c3->SetGridy();
    c3->SetWindowSize(w + (w - c3->GetWw()), h + (h - c3->GetWh()));
    c3->SetRightMargin(0.25);
    c3->SetLeftMargin(0.2);
    c3->SetBottomMargin(0.2);

    gStyle->SetPalette(kRainBow);
    gStyle->SetNumberContours(99);
    
    gr->SetNpx(120);
    gr->SetNpy(4000);
    //gr->SetMaxIter(500000);
    gPad->DrawFrame(-120., -1000., 120., 1000., title.c_str());

    gr->SetNpx(120);
    gr->SetNpy(4000);
    gr->Draw("colz same");
    gr->GetHistogram()->GetXaxis()->SetRangeUser(-120., 120.);
    gr->GetHistogram()->GetYaxis()->SetRangeUser(-200., 200.);

    gr->GetHistogram()->GetXaxis()->CenterTitle(true);
    gr->GetHistogram()->GetYaxis()->CenterTitle(true);
    gr->GetHistogram()->GetZaxis()->CenterTitle(true);

    gr->GetHistogram()->GetXaxis()->SetTitleSize(0.05); // 坐标轴标题的大小
    gr->GetHistogram()->GetYaxis()->SetTitleSize(0.05);
    gr->GetHistogram()->GetZaxis()->SetTitleSize(0.05);

    gr->GetHistogram()->GetXaxis()->SetLabelSize(0.05); // 坐标轴刻度值的大小
    gr->GetHistogram()->GetYaxis()->SetLabelSize(0.05);

    gPad->Update();
    c3->Update();

    string houzhui1 = ".png";
    myFile->WriteObject(c3, (name + type).c_str());
    c3->SaveAs((name + type + houzhui1).c_str());
    
    delete c3;
    
}


int main()
{
    std::cout << "Type the directory name." << std::endl;
    std::cin >> dir1;
    const string dir2 = "./" + dir1;

    Float_t x, y, z, xx, yy, Px, Py, Pz, t, PDGid, EventID, TrackID, ParentID, Weight, energy;
    Float_t Element[NoEle] = {x, y, z, Px, Py, Pz, t, PDGid, EventID, TrackID, ParentID, Weight};

    //TFile *test01 = new TFile("test01.root", "RECREATE");

    const char* Elements[NoEle] = {"x", "y", "z", "Px", "Py", "Pz", "t", "PDGid", "EventID", "TrackID", "ParentID", "Weight"};

    vector<string> fileList;

    scanFiles(fileList, dir2);

    TChain** chN = new TChain*[NoDet];
    
    for(int j = 0; j < NoDet; j++)
    {   
        int numOf01 = 0,numOf02 = 0,numOf03 = 0,numOf04 = 0,numOf1 = 0,numOf2 = 0,numOf3 = 0,numOf4 = 0,numOf5 = 0, numOf10 = 0,numOf15 = 0,numOf20 = 0,numOf40 = 0,numOf60 = 0,numOf80 = 0,numOf100 = 0,numOf120 = 0,numOf140 = 0,numOf160 = 0,numOf180 = 0,numOf200 = 0;
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

        //TH1F *xDistr = new TH1F("xDistr", "X distribution", 240, -120, 120);
        //TH1F *yDistr = new TH1F("yDistr", "Y distribution", 240, -120, 120);
        //TH1F *energyDistr = new TH1F("hist", "Energy distribution after degrader", 1600, 0, 1600);
        //TH2F *x_PPx = new TH2F("x_PPx", "X-X' distribution; X (mm); X' (mrad); Entries", 240, -120, 120, 400, -1000, 1000);
        //TH2F *y_PPy = new TH2F("y_PPy", "Y-Y' distribution; Y (mm); Y' (mrad); Entries", 240, -120, 120, 400, -1000, 1000);
        //TH2F *x_y = new TH2F("x_y", "X-Y distribution; X (mm); Y (mm); Entries", 240, -120, 120, 240, -120, 120);
        //TGraph2D *x_xx_e = new TGraph2D(); string title1 = "x, x' and energy distribution; x (mm); x' (mrad); Energy (MeV)";
        //TGraph2D *y_yy_e = new TGraph2D(); string title2 = "y, y' and energy distribution; y (mm); y' (mrad); Energy (MeV)";
        
        for(int k = 0; k < entries; k++)
        {
            chN[j]->GetEntry(k);
            energy = TMath::Sqrt(938.27231*938.27231+Element[3]*Element[3]+Element[4]*Element[4]+Element[5]*Element[5])-938.27231;
            x = Element[0];
            y = Element[1]-512.106;
            xx = 1000*Element[3]/Element[5];
            yy = 1000*Element[4]/Element[5];

            //xDistr->Fill(x);
            //yDistr->Fill(y);
            //x_PPx->Fill(x, xx);
            //y_PPy->Fill(y, yy);
            //x_y->Fill(x, y);
            //x_xx_e->SetPoint(x_xx_e->GetN(), Double_t(x),  Double_t(xx),  Double_t(energy));
            //y_yy_e->SetPoint(y_yy_e->GetN(),  Double_t(y),  Double_t(yy),  Double_t(energy));
            
            if (x*x+y*y <=  1 )    {numOf01 ++;}
            if (x*x+y*y <=  2 )    {numOf02 ++;}
            if (x*x+y*y <=  3 )    {numOf03 ++;}
            if (x*x+y*y <=  4 )    {numOf04 ++;}
            if (x*x+y*y <=  5 )    {numOf1 ++;}
            if (x*x+y*y <=  6 )    {numOf2 ++;}
            if (x*x+y*y <=  7 )    {numOf3 ++;}
            if (x*x+y*y <=  8 )    {numOf4 ++;}
            if (x*x+y*y <=  9 )    {numOf5 ++;}
            if (x*x+y*y <=  10)    {numOf10++;}
            if (x*x+y*y <=  15)    {numOf15++;}
            if (x*x+y*y <=  20)    {numOf20++;} 
            if (x*x+y*y <=  40)    {numOf40++;}
            if (x*x+y*y <=  60)    {numOf60++;}
            if (x*x+y*y <=  80)    {numOf80++;}
            if (x*x+y*y <= 100)    {numOf100++;}
            if (x*x+y*y <= 120)    {numOf120++;}
            if (x*x+y*y <= 140)    {numOf140++;} 
            if (x*x+y*y <= 160)    {numOf160++;}
            if (x*x+y*y <= 180)    {numOf180++;}
            if (x*x+y*y <= 200)    {numOf200++;}
            
        }
        //TCanvas *cc = new TCanvas();
        //chN[j]->Draw("x:y:Px","","colz");
        //test01->Write();
        //delete cc;
        Double_t percentage001 = double(numOf01)/double(entries);
        Double_t percentage002 = double(numOf02)/double(entries);
        Double_t percentage003 = double(numOf03)/double(entries);
        Double_t percentage004 = double(numOf04)/double(entries);
        Double_t percentage01 = double(numOf1)/double(entries);
        Double_t percentage02 = double(numOf2)/double(entries);
        Double_t percentage03 = double(numOf3)/double(entries);
        Double_t percentage04 = double(numOf4)/double(entries);
        Double_t percentage1 = double(numOf5)/double(entries);
        Double_t percentage2 = double(numOf10)/double(entries);
        Double_t percentage3 = double(numOf15)/double(entries);
        Double_t percentage4 = double(numOf20)/double(entries);
        Double_t percentage5 = double(numOf40)/double(entries);
        Double_t percentage6 = double(numOf60)/double(entries);
        Double_t percentage7 = double(numOf80)/double(entries);
        Double_t percentage8 = double(numOf100)/double(entries);
        Double_t percentage9 = double(numOf120)/double(entries);
        Double_t percentage10 = double(numOf140)/double(entries);
        Double_t percentage11 = double(numOf160)/double(entries);
        Double_t percentage12 = double(numOf180)/double(entries);
        Double_t percentage13 = double(numOf200)/double(entries);

        //DrawOneDim(xDistr, yDistr, DetNames[j]);
//
        //DrawTwoDim(x_y, DetNames[j], "x_y");
        //DrawTwoDim(x_PPx, DetNames[j], "x_PPx");
        //DrawTwoDim(y_PPy, DetNames[j], "y_PPy");

        //DrawThreeDim(x_xx_e, DetNames[j], "x_xx_e", title1);
        //DrawThreeDim(y_yy_e, DetNames[j], "y_yy_e", title2);

        std::cout << DetNames[j] << ": "  << std::endl;
        std::cout << "1*1 占总数比例: " << percentage001  << std::endl;
        std::cout << "2*2 占总数比例: " << percentage002  << std::endl;
        std::cout << "3*3 占总数比例: " << percentage003  << std::endl;
        std::cout << "4*4 占总数比例: " << percentage004  << std::endl;
        std::cout << "5*5 占总数比例: " << percentage01  << std::endl;
        std::cout << "6*6 占总数比例: " << percentage02  << std::endl;
        std::cout << "7*7 占总数比例: " << percentage03  << std::endl;
        std::cout << "8*8 占总数比例: " << percentage04  << std::endl;
        std::cout << "9*9 占总数比例: "  << percentage1  << std::endl;
        std::cout << "10*10 占总数比例: " << percentage2  << std::endl;
        std::cout << "15*15 占总数比例: " << percentage3  << std::endl;
        std::cout << "20*20 占总数比例: " << percentage4  << std::endl;
        std::cout << "40*40 占总数比例: " << percentage5  << std::endl;
        std::cout << "60*60 占总数比例: " << percentage6  << std::endl;
        std::cout << "80*pi 占总数比例: " << percentage7  << std::endl;
        std::cout << "100*100 占总数比例: " << percentage8  << std::endl;
        std::cout << "120*120 占总数比例: " << percentage9  << std::endl;
        std::cout << "140*140 占总数比例: " << percentage10  << std::endl;
        std::cout << "160*160 占总数比例: " << percentage11 << std::endl;
        std::cout << "180*180 占总数比例: " << percentage12  << std::endl;
        std::cout << "200*200 占总数比例: " << percentage13  << std::endl;

        //delete xDistr;
        //delete yDistr;
        //delete energyDistr;
        //delete x_PPx;
        //delete y_PPy;
        //delete x_y;
        //delete x_xx_e;
        //delete y_yy_e;
        
    }
    myFile->Close();
    //test01->Close();

    delete[] chN;
    return 0;
}