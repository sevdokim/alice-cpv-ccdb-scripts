#if !defined(__CLING__) || defined(__ROOTCLING__)
#include "CCDB/BasicCCDBManager.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "CCDB/CCDBTimeStampUtils.h"
#include "DataFormatsPHOS/CalibParams.h"
#include "Base/Geometry.h"
#include <iostream>
#include <fstream>
#endif

o2::phos::CalibParams* readPHSCalibParamsFromCCDB(const char* ccdbURI = "http://o2-ccdb.internal", long timeStamp = 0)
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
  o2::phos::CalibParams* cp = ccdbMgr.get<o2::phos::CalibParams>("PHS/Calib/CalibParams");
  if (!cp) {
    std::cerr << "Cannot get CalibParams from CCDB/PHS/Calib/CalibParams!" << std::endl;
    return 0x0;
  }

  TCanvas *c = new TCanvas("Modules","Modules");
  c->Divide(2,2);
  for(int m = 1; m <= 4; m++){
    c->cd(m);
    TH2F * h = new TH2F(Form("HGtoLG%d",m),Form("HG/LG in M = %d",m),64,0,64,56,0.,56.);
    h->GetXaxis()->SetTitle("X crystal");
    h->GetYaxis()->SetTitle("Z crystal");
    char relid[3];
    for(short absId=32*56+1;absId<=14336; absId++){
      o2::phos::Geometry::absToRelNumbering(absId,relid);
      if(relid[0]==m){
	h->SetBinContent(relid[1],relid[2],cp->getHGLGRatio(absId)) ;
	
      }
    }
    h->Draw("colz") ;
  }
  
  
  return cp;
}
