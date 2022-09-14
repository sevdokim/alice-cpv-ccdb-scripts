#if !defined(__CLING__) || defined(__ROOTCLING__)
#include "CCDB/BasicCCDBManager.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "CCDB/CCDBTimeStampUtils.h"
#include "CPVBase/Geometry.h"
#endif

void readThresholdsFEEFromCCDBAndPushBack(const char* ccdbURI = "localhost:8084", long timeStamp = 0, const char* destinationHost = "http://alio2-cr1-flp199:8083", long destinationTimeStamp = 0)
{
  // ccdbURI -> CCDB instance
  // timeStamp -> time in milliseconds (wow!) (starting at 00:00 on 1.1.1970)
  // timeStamp == 0 -> current time

  // read from source
  auto& ccdbMgr = o2::ccdb::BasicCCDBManager::instance();
  ccdbMgr.setURL(ccdbURI);
  if (!ccdbMgr.isHostReachable()) {
    std::cerr << ccdbURI << " is not reachable!" << std::endl;
    return;
  }
  if (timeStamp == 0) {
    timeStamp = o2::ccdb::getCurrentTimestamp();
  }
  ccdbMgr.setTimestamp(timeStamp);
  std::vector<int>* thrs = ccdbMgr.get<std::vector<int>>("CPV/PedestalRun/FEEThresholds");
  if (!thrs) {
    std::cerr << "Cannot get FEE thresholds from CCDB/CPV/PedestalRun/FEEThresholds!" << std::endl;
    return;
  }

  // draw thresholds and modify
  TH2I* hThresholdsValues[3];
  o2::cpv::Geometry geo;
  short relId[3];
  std::ofstream pededstalsTxt;
  pededstalsTxt.open("pedestals.txt");
  for (int iMod = 0; iMod < 3; iMod++) {
    hThresholdsValues[iMod] = new TH2I(Form("hThresholdsM%d", iMod + 2),
                                       Form("Threshold values in M%d", iMod + 2),
                                       128, 0., 128., 60, 0., 60);
    for (int iCh = iMod * 7680; iCh < (iMod + 1) * 7680; iCh++) {
      geo.absToRelNumbering(iCh, relId);
      // modify thrs
      if (relId[0] == 2 && relId[1] <= 16) thrs->at(iCh) = 511;
      hThresholdsValues[iMod]->SetBinContent(relId[1] + 1, relId[2] + 1, thrs->at(iCh) & 0xffff);
      pededstalsTxt << (thrs->at(iCh) >> 16) << " " << (thrs->at(iCh) & 0xffff) << std::endl;
    }
    TCanvas* can = new TCanvas(Form("canM%d", iMod + 2), Form("module M%d", iMod + 2), 10 * iMod, 0, 1000 + 10 * iMod, 1000);
    can->Divide(1, 1);
    can->cd(1);
    hThresholdsValues[iMod]->Draw("colz");
  }
  pededstalsTxt.close();

  // store thresholds
  o2::ccdb::CcdbApi ccdb;
  map<string, string> metadata;
  metadata["Uploader"] = "sevdokim";
  metadata["Description"] = Form("FEEThresholds%ld", timeStamp);
  ccdb.init(destinationHost);
  destinationTimeStamp = destinationTimeStamp > 0 ? destinationTimeStamp : o2::ccdb::getCurrentTimestamp();
  std::cout << "Going to store " << thrs << " with validity time = (" << destinationTimeStamp << "; " << destinationTimeStamp + 100000000 << ")\n";
  ccdb.storeAsTFileAny(thrs, "CPV/PedestalRun/FEEThresholds", metadata, destinationTimeStamp, destinationTimeStamp + 1000000000);
}
