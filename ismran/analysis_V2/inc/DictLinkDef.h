// See: https://root.cern.ch/selecting-dictionary-entries-linkdefh
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedef;

#pragma link C++ namespace lite_interface;
#pragma link C++ defined_in namespace lite_interface;

#pragma link C++ class lite_interface::Point3D+;
#pragma link C++ class std::vector<lite_interface::Point3D*>+;
#pragma link C++ class lite_interface::ScintillatorBar_V2+;
#pragma link C++ class std::vector<lite_interface::ScintillatorBar_V2*>+;
#pragma link C++ class lite_interface::SingleMuonTrack+;
#endif
