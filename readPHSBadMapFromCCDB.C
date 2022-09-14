#if !defined(__CLING__) || defined(__ROOTCLING__)
#include "CCDB/BasicCCDBManager.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "CCDB/CCDBTimeStampUtils.h"
#include "DataFormatsPHOS/BadChannelsMap.h"
#include "Base/Geometry.h"
#include <iostream>
#include <fstream>
#endif

o2::phos::BadChannelsMap* readPHSBadMapFromCCDB(const char* ccdbURI = "http://o2-ccdb.internal", long timeStamp = 0)
{
  gStyle->SetOptStat(0);
  
  auto& ccdbMgr = o2::ccdb::BasicCCDBManager::instance();
  ccdbMgr.setURL(ccdbURI);
  if (!ccdbMgr.isHostReachable()) {
    std::cerr << ccdbURI << " is not reachable!" << std::endl;
    return 0x0;
  }
  if (timeStamp == 0) {
    timeStamp = o2::ccdb::getCurrentTimestamp();
  }
  ccdbMgr.setTimestamp(timeStamp);
  o2::phos::BadChannelsMap* cp = ccdbMgr.get<o2::phos::BadChannelsMap>("PHS/Calib/BadMap");
  if (!cp) {
    std::cerr << "Cannot get PHOS Bad channel map from CCDB/PHS/Calib/BadMap!" << std::endl;
    return 0x0;
  }

  gStyle->SetPadRightMargin(0.02);
  gStyle->SetPadLeftMargin(0.09);
  gStyle->SetPadBottomMargin(0.08);
  gStyle->SetPadTopMargin(0.07);
  TCanvas *c = new TCanvas("Modules","Modules",100,100,1200,800);
  c->Divide(2,2);
  for(int m = 1; m <= 4; m++){
    c->cd(m);
    gPad->SetGridx();
    gPad->SetGridy();
    TH2F * h = new TH2F(Form("BadMap%d",m),Form("Bad channel map in M = %d",m),64,0,64,56,0.,56.);
    h->SetNdivisions(-504,"X");
    h->SetNdivisions(-528,"Y");
    h->SetTitleSize(0.02,"XY");
    h->SetLabelSize(0.01,"XY");
    h->SetLabelOffset(0.02,"X");
    h->GetXaxis()->SetTitle("X crystal");
    h->GetYaxis()->SetTitle("Z crystal");

    char relid[3];
    for(short absId=32*56+1;absId<=14336; absId++){
      o2::phos::Geometry::absToRelNumbering(absId,relid);
      if(relid[0]==m){
	h->SetBinContent(relid[1],relid[2],!cp->isChannelGood(absId)) ;
      }
    }
    h->Draw("box") ;
  }
  
  c->Print("PHS_BadMap.png");
  return cp;
}
