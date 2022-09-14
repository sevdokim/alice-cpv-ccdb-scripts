void pushCPVSimParams()
{
  o2::ccdb::CcdbApi ccdb;
  map<string, string> metadata;
  metadata["Uploader"] = "sevdokim";
  ccdb.init("http://o2-ccdb.internal");
  //ccdb.init("http://ccdb-test.cern.ch:8080");
  auto& o2CpvSP = o2::cpv::CPVSimParams::Instance(); // default
  //o2::conf::ConfigurableParam::setValue("CPVSimParams.mDigitMinEnergy", "5.01");
  o2CpvSP.printKeyValues();
  ccdb.storeAsTFileAny(&o2CpvSP, "CPV/Config/CPVSimParams", metadata, 946684800000, 3385078236000);
}
