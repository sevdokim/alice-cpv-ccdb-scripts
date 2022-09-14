void pushDefaultCCDBObjectsCPV()
{
  o2::ccdb::CcdbApi ccdb;
  map<string, string> metadata;
  ccdb.init("http://o2-ccdb.internal");
  auto o2cpvBM = new o2::cpv::BadChannelMap(1); //ideal bad map
  ccdb.storeAsTFileAny(o2cpvBM, "CPV/Calib/BadChannelMap", metadata, 1546300800000, 1554900770000);
  auto o2cpvCalib = new o2::cpv::CalibParams(1); //ideal
  ccdb.storeAsTFileAny(o2cpvCalib, "CPV/Calib/Gains", metadata, 1546300800000, 1554900770000);
  auto o2cpvPeds = new o2::cpv::Pedestals(1); //ideal
  ccdb.storeAsTFileAny(o2cpvPeds, "CPV/Calib/Pedestals", metadata, 1546300800000, 1554900770000);
}
