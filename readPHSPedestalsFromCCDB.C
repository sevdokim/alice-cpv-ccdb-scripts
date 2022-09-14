#if !defined(__CLING__) || defined(__ROOTCLING__)
#include "CCDB/BasicCCDBManager.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "CCDB/CCDBTimeStampUtils.h"
#include "DataFormatsPHOS/Pedestals.h"
#include "Base/Geometry.h"
#include <iostream>
#include <fstream>
#endif

o2::phos::Pedestals* readPHSPedestalsFromCCDB(const char* ccdbURI = "localhost:8084", long timeStamp = 0, bool isHG = 1)
{
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
  o2::phos::Pedestals* ped = ccdbMgr.get<o2::phos::Pedestals>("PHS/Calib/Pedestals");
  if (!ped) {
    std::cerr << "Cannot get pedestals from CCDB/PHS/Calib/Pedestals!" << std::endl;
    return 0x0;
  }

  TCanvas *c = new TCanvas("Modules","Modules");
  c->Divide(2,2);
  for(int m = 1; m <= 4; m++){
    c->cd(m);
    TH2F * h = new TH2F(Form("Pedestals%d",m),Form("Pedestals for module %d",m),64,0,64,56,0.,56.);
    h->GetXaxis()->SetTitle("X crystall");
    h->GetYaxis()->SetTitle("Z crystall");
    char relid[3];
    for(short absId=32*56+1;absId<=14336; absId++){
      o2::phos::Geometry::absToRelNumbering(absId,relid);
      if(relid[0]==m){
      	if(isHG)
        	h->SetBinContent(relid[1],relid[2],ped->getHGPedestal(absId)) ;
        else
        	h->SetBinContent(relid[1],relid[2],ped->getLGPedestal(absId)) ;

      }
    }
    h->Draw("colz") ;
  }
  
  
  return ped;
}
