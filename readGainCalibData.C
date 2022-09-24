#if !defined(__CLING__) || defined(__ROOTCLING__)
#include "CCDB/BasicCCDBManager.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "CCDB/CCDBTimeStampUtils.h"
#include "CPVCalibration/GainCalibrator.h"
#include "CPVBase/Geometry.h"
#include <iostream>
#endif

TH1F* hAmplitude[5][128][60] = {0x0};

o2::cpv::GainCalibData* readGainCalibData(const char* ccdbURI = "http://o2-ccdb.internal", long timeStamp = 0)
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
  o2::cpv::GainCalibData* gcd = ccdbMgr.get<o2::cpv::GainCalibData>("CPV/PhysicsRun/GainCalibData");
  if (!gcd) {
    std::cerr << "Cannot get GainCalibData from CPV/PhysicsRun/GainCalibData!" << std::endl;
    return 0x0;
  }
  gcd->print();

  cout << "I passed print" << endl;
  TH2F *hGCDEntries[3], *hGCDMean[3];
  o2::cpv::Geometry geo;
  short relId[3];
  short ix, iz;
  TCanvas* can = new TCanvas("Modules", "Modules");
  can->Divide(3,2);
  for (int iMod = 0; iMod < 3; iMod++) {
    hGCDEntries[iMod] = new TH2F(Form("hGCDEntriesM%d", iMod + 2),
                                 Form("GainCalibData entries in M%d", iMod + 2),
                                 128, 0., 128., 60, 0., 60);
    hGCDMean[iMod] = new TH2F(Form("hGCDMeanM%d", iMod + 2),
                                 Form("GainCalibData means in M%d", iMod + 2),
                                 128, 0., 128., 60, 0., 60);
    for (int iCh = iMod * 7680; iCh < (iMod + 1) * 7680; iCh++) {
      geo.absToRelNumbering(iCh, relId);
      ix = relId[1];
      iz = relId[2];
      hGCDEntries[iMod]->SetBinContent(relId[1] + 1, relId[2] + 1, gcd->mAmplitudeSpectra[iCh].getNEntries());
      hAmplitude[iMod][ix][iz] = new TH1F(Form("hAmplitude_mod%d_x%d_z%d", iMod, ix, iz), Form("Amplitudes in module %d, X = %d, Z = %d", iMod, ix+1, iz+1), 1000, 0., 1000.);
      if (gcd->mAmplitudeSpectra[iCh].getNEntries() > 0) {
        hGCDMean[iMod]->SetBinContent(relId[1] + 1, relId[2] + 1, gcd->mAmplitudeSpectra[iCh].getMean());
	gcd->mAmplitudeSpectra[iCh].fillBinData(hAmplitude[iMod][ix][iz]);
      }
    }
    can->cd(iMod + 1);
    gPad->AddExec("da", "drawAmplitude()");
    hGCDEntries[iMod]->GetXaxis()->SetTitle("X pad");
    hGCDEntries[iMod]->GetYaxis()->SetTitle("Z pad");
    hGCDEntries[iMod]->Draw("colz");
    can->cd(iMod + 4);
    hGCDMean[iMod]->GetXaxis()->SetTitle("X pad");
    hGCDMean[iMod]->GetYaxis()->SetTitle("Z pad");
    hGCDMean[iMod]->Draw("colz");
    gPad->AddExec("da", "drawAmplitude()");
  }
  return gcd;
}
void drawAmplitude()
{
  TObject *select = gPad->GetSelected();
  if(!select) return;
  if (!select->InheritsFrom("TH2")) {gPad->SetUniqueID(0); return;}
  gPad->GetCanvas()->FeedbackMode(kTRUE);
  if(gPad->GetEvent()!=kButton1Up)return ;
  Int_t clickModule=-1 ;
  clickModule = 1 + gPad->GetNumber();
  if(clickModule < 2 || clickModule > 4) return ;
  
  //erase old position and draw a line at current position
  int px=gPad->GetEventX() ;
  int py=gPad->GetEventY() ;
  gPad->SetUniqueID(py);
  Float_t upy = gPad->AbsPixeltoY(py);
  Float_t y   = gPad->PadtoY(upy);
  Float_t upx = gPad->AbsPixeltoX(px);
  Float_t x   = gPad->PadtoX(upx);
  TH2 *h = (TH2*)select;
  if(!h) return ;
  Int_t cellZ = h->GetYaxis()->FindBin(y) ;
  Int_t cellX = h->GetXaxis()->FindBin(x) ;
  printf("Module=%d, pad (x,z)=(%d,%d)\n",clickModule,cellX,cellZ);
  
  TCanvas *cAmpl = new TCanvas("cAmpl", "Amplitude ");
  cAmpl->cd();
  hAmplitude[clickModule - 2][cellX - 1][cellZ - 1]->Draw();
}
