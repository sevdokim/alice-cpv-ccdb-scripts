void pushDefaultCCDBObjectsPHS()
{
  o2::ccdb::CcdbApi ccdb;
  map<string, string> metadata;
  ccdb.init("http://o2-ccdb.internal");
  auto o2phosBM = new o2::phos::BadChannelsMap();
  ccdb.storeAsTFileAny(o2phosBM, "PHS/Calib/BadMap", metadata, -1, 1670700184549);
  auto o2phosCalib = new o2::phos::CalibParams(1);
  ccdb.storeAsTFileAny(o2phosCalib, "PHS/Calib/CalibParams", metadata, -1, 1670700184549);
  auto o2phosPeds = new o2::phos::Pedestals();
  ccdb.storeAsTFileAny(o2phosPeds, "PHS/Calib/Pedestals", metadata, -1, 1670700184549);
}
