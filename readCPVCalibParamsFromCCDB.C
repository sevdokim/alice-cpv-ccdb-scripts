#if !defined(__CLING__) || defined(__ROOTCLING__)
#include "CCDB/BasicCCDBManager.h"
#include "CCDB/CCDBTimeStampUtils.h"
#include "CPVBase/CPVCalibParams.h"
#include <iostream>
#endif

void readCPVCalibParamsFromCCDB(const char* ccdbURI = "http://o2-ccdb.internal", long timeStamp = 0)
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
  o2::cpv::CPVCalibParams* cp = ccdbMgr.get<o2::cpv::CPVCalibParams>("CPV/Config/CPVCalibParams");
  if (!cp) {
    std::cerr << "Cannot get gains from CCDB/CPV/Config/CPVCalibParams!" << std::endl;
    return 0x0;
  }
  cp->printKeyValues();
  std::cout << "Don't believe [ CODE ] label. Parameters are taken from CCDB." << std::endl;
}
