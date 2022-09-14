void pushDefaultGainCalibData()
{
  o2::ccdb::CcdbApi ccdb;
  map<string, string> metadata;
  metadata["uploader"] = "sevdokim";
  metadata["what"] = "empty GainCalibData";
  ccdb.init("http://o2-ccdb.internal");
  //ccdb.init("http://ccdb-test.cern.ch:8080");
  auto o2cpvGCD = new o2::cpv::GainCalibData(); //empty
  ccdb.storeAsTFileAny(o2cpvGCD, "CPV/PhysicsRun/GainCalibData", metadata, 946684800000, 3385078236000);
}
