void pushPersistentBadChannels(const char* destinationHost = "http://o2-ccdb.internal", long destinationTimeStamp = -1)
{
  o2::cpv::Geometry geo;
  // fill bad channels
  std::vector<int> badChs;
  std::vector<int> badChsEmpty;
  short ccId, dil, gas, pad, relId[3];
  for (int iCh = 0; iCh <  geo.kNCHANNELS; iCh++) {
    geo.absIdToHWaddress(iCh, ccId, dil, gas, pad);
    geo.absToRelNumbering(iCh, relId);
    // M2, СС = 0
    if (ccId == 0) badChs.push_back(iCh);
    // M4, CC = 17 Dil = 0
    if (ccId == 17 && dil == 1) badChs.push_back(iCh);
    // M4, CC = 23 Dil = 0 Gas = 0,1
    if (ccId == 23 && dil == 0 && gas <= 1) badChs.push_back(iCh);
    // add more bad channels below
  }

  //draw resulting bad map
  TH2I* hBadMap[3];
  for (int iMod = 0; iMod < 3; iMod++) {
    hBadMap[iMod] = new TH2I(Form("hBadMapM%d", iMod + 2),
			     Form("Bad Channel Map in M%d", iMod + 2),
			     128, -0.5, 127.5, 60, -0.5, 59.5);
  }
  for (auto badCh : badChs) {
    geo.absToRelNumbering(badCh, relId);
    hBadMap[relId[0] - 2]->Fill(relId[1], relId[2]);
  }
  for (int iMod = 0; iMod < 3; iMod++) {
    TCanvas* c1 = new TCanvas();
    hBadMap[iMod]->Draw("colz");
  }
  
  // store badChs in ccdb/CPV/Config/PersistentBadChannels
  o2::ccdb::CcdbApi ccdb;
  map<string, string> metadata;
  metadata["Uploader"] = "sevdokim";
  metadata["Description"] = "AprioriKnownBadChannels";
  ccdb.init(destinationHost);
  destinationTimeStamp = destinationTimeStamp > 0 ? destinationTimeStamp : o2::ccdb::getCurrentTimestamp();
  std::cout << "Going to store " << badChs.size() << " bad channels with validity time = (" << destinationTimeStamp << "; " << 3385078236000 << ")\n";
  // ccdb.storeAsTFileAny(&badChs, "CPV/Config/PersistentBadChannels", metadata, destinationTimeStamp, 3385078236000);
  ccdb.storeAsTFileAny(&badChs, "CPV/Config/PersistentBadChannels", metadata, 1653609600000, 1663260047597);
  metadata["Description2"] = "EmptyListOfChannels";
  ccdb.storeAsTFileAny(&badChsEmpty, "CPV/Config/PersistentBadChannels", metadata, 1546300800000, 1554900770000);
}
