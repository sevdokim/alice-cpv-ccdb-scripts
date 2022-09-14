void pushCPVCalibParams()
{
  o2::ccdb::CcdbApi ccdb;
  map<string, string> metadata;
  metadata["Uploader"] = "sevdokim";
  ccdb.init("http://o2-ccdb.internal");
  //ccdb.init("http://ccdb-test.cern.ch:8080");
  auto& o2CpvCP = o2::cpv::CPVCalibParams::Instance(); // default
  //o2::conf::ConfigurableParam::setValue("CPVSimParams.mDigitMinEnergy", "5.01");
  o2::conf::ConfigurableParam::setValue("CPVCalibParams.noiseFrequencyCriteria", "0.1");
  o2::conf::ConfigurableParam::setValue("CPVCalibParams.noiseThreshold", "10");
  o2CpvCP.printKeyValues();
  //ccdb.storeAsTFileAny(&o2CpvCP, "CPV/Config/CPVCalibParams", metadata, 946684800000, 3385078236000);
  ccdb.storeAsTFileAny(&o2CpvCP, "CPV/Config/CPVCalibParams", metadata, -1, 3385078236000);
}
