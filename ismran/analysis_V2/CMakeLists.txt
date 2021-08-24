cmake_minimum_required(VERSION 2.8)
project(MST)

#Added to detect compiler support for C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

option(CRY "Enable CRY library for cosmic event generation" OFF)
option(USE_CALIBRATION "Use Calibration data for Sampling of time" OFF)
option(USE_FOR_SIMULATION "Use Analyzer for simulated data" OFF)
option(STAGGERED_GEOM "Use Scattered geometry configurtion" OFF)
option(SINGLE_POINT_CALIBRATION "Use Single Point Muon Calibration" ON)
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
option(FINGER_PATTERN_CONDITION "Flag to see the finger plot of layers" OFF)
option(VERTICAL_SIGNAL_CABLE "Flag to use the code for Vertical arrangement" OFF)
option(USE_MUON_CALIB "Flag to use Muon based calibration" OFF)

if(USE_MUON_CALIB)
add_definitions(-DUSE_MUON_CALIB)
endif(USE_MUON_CALIB)

if(VERTICAL_SIGNAL_CABLE)
add_definitions(-DVERTICAL_ARRANGEMENT)
endif(VERTICAL_SIGNAL_CABLE)


if(FINGER_PATTERN_CONDITION)
add_definitions(-DUSE_CRITICAL_COND)
endif(FINGER_PATTERN_CONDITION)

if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

if(SINGLE_POINT_CALIBRATION)
add_definitions(-DSINGLE_POINT_CALIBRATION)
endif(SINGLE_POINT_CALIBRATION)


if(STAGGERED_GEOM)
add_definitions(-DSTAGGERED_GEOM)
endif(STAGGERED_GEOM)

if(CRY)
add_definitions(-DUSE_CRY)
endif(CRY)

if(USE_FOR_SIMULATION)
add_definitions(-DUSE_FOR_SIMULATION)
endif(USE_FOR_SIMULATION)

if(USE_CALIBRATION)
add_definitions(-DUSE_CALIBRATION)
endif(USE_CALIBRATION)

#----------------------------------------------------------------------------
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})

file(GLOB MAC_FILES ${PROJECT_SOURCE_DIR}/MacFiles/*)
message(STATUS ${MAC_FILES})
#file(COPY ${MAC_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/build)
file(COPY ${MAC_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(../..)
include_directories(../inc)
include_directories(../../base)
include_directories(../ml)
include_directories(../../CoordinateGenerator/inc)
include_directories(../../TemplatizedInterface/inc)

#Block to generate visualizer library
include_directories(../visualizer/Eve/inc)
include_directories(../visualizer)
include_directories(../../Helpers/inc)
file(GLOB visualizerSources ../visualizer/Eve/src/*.cpp ../visualizer/*.cpp ../../Helpers/src/CommonFunc.cpp ../../Helpers/src/Track.cpp)
add_library(Visualizer SHARED ${visualizerSources})
target_link_libraries(Visualizer ${ROOT_LIBRARIES} Eve Geom)


#ROOT_GENERATE_DICTIONARY(G__Tomo ../inc/Point3D.h LINKDEF ../inc/DictLinkDef.h)
ROOT_GENERATE_DICTIONARY(G__Tomo ../inc/Point3D.h ../inc/ScintillatorBar_V2.h ../inc/SingleMuonTrack.h LINKDEF ../inc/DictLinkDef.h)
add_library(Tracklib SHARED ../src/ScintillatorBar_V2.cpp ../src/SingleMuonTrack.cpp ../src/Point3D.cpp G__Tomo.cxx)
target_link_libraries(Tracklib ${ROOT_LIBRARIES})


file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp  ../inc/*.cpp ../ml/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/inc/*.h ../../base/Vector3D.h ../ml/ml.h) 

add_executable(AnalyzeData test/AnalyzeData.cpp ${sources} ${headers})
target_link_libraries(AnalyzeData ${ROOT_LIBRARIES} ${TOMOLIB} Tracklib)

add_executable(ReadTracks test/ReadTracks.cpp ${sources} ${headers})
target_link_libraries(ReadTracks ${ROOT_LIBRARIES}  Tracklib)

add_executable(ReadTracks2 test/ReadTracks2.cpp ${sources} ${headers})
target_link_libraries(ReadTracks2 ${ROOT_LIBRARIES}  Tracklib)

add_executable(ReadTracks3 test/ReadTracks3.cpp ${sources} ${headers})
target_link_libraries(ReadTracks3 ${ROOT_LIBRARIES}  Tracklib)

add_executable(ReadTracks4 test/ReadTracks4.cpp ${sources} ${headers})
target_link_libraries(ReadTracks4 ${ROOT_LIBRARIES}  Tracklib)

add_executable(TestPixelFromBars test/TestPixelFromBars.cpp ${sources} ${headers})
target_link_libraries(TestPixelFromBars ${ROOT_LIBRARIES}  Tracklib)

add_executable(TestScintillatorHealth test/TestScintillatorHealth.cpp ${sources} ${headers})
target_link_libraries(TestScintillatorHealth ${ROOT_LIBRARIES}  Tracklib)

add_executable(TestScintillatorPeaks test/TestScintillatorPeaks.cpp ${sources} ${headers})
target_link_libraries(TestScintillatorPeaks ${ROOT_LIBRARIES}  Tracklib)

add_executable(TestTimeToPositionResolution test/TestTimeToPositionResolution.cpp ${sources} ${headers})
target_link_libraries(TestTimeToPositionResolution ${ROOT_LIBRARIES}  Tracklib)

add_executable(ReadTracksDebug test/ReadTracksDebug.cpp ${sources} ${headers})
target_link_libraries(ReadTracksDebug ${ROOT_LIBRARIES}  Tracklib)

add_executable(ReadVect3DTracks test/ReadVect3DTracks.cpp ${sources} ${headers})
target_link_libraries(ReadVect3DTracks ${ROOT_LIBRARIES} Tracklib)

add_executable(ReadProcessedData test/ReadProcessedData.cpp ${sources} ${headers})
target_link_libraries(ReadProcessedData ${ROOT_LIBRARIES} Tracklib)

add_executable(ReadProcessedData2 test/ReadProcessedData2.cpp ${sources} ${headers})
target_link_libraries(ReadProcessedData2 ${ROOT_LIBRARIES} Tracklib)

add_executable(ReadExactHitData test/ReadExactHitData.cpp ${sources} ${headers})
target_link_libraries(ReadExactHitData ${ROOT_LIBRARIES} Tracklib)

add_executable(TestSystemHealth test/TestSystemHealth.cpp ${sources} ${headers})
target_link_libraries(TestSystemHealth ${ROOT_LIBRARIES} Tracklib)

add_executable(SystemHealthComparison test/SystemHealthComparison.cpp ${sources} ${headers})
target_link_libraries(SystemHealthComparison ${ROOT_LIBRARIES} Tracklib)

#add_executable(GetEnergyCalibrationFormulaFromCosmicData test/GetEnergyCalibrationFormulaFromCosmicData.cpp ${sources} ${headers})
#target_link_libraries(GetEnergyCalibrationFormulaFromCosmicData ${ROOT_LIBRARIES} Tracklib -lSpectrum)

#add_executable(ReadTracks_V2 test/ReadTracks_V2.cpp ${sources} ${headers})
#target_link_libraries(ReadTracks_V2 ${ROOT_LIBRARIES}  Tracklib)

add_executable(readCalibrationFormulaFile test/readCalibrationFormulaFile.cpp ${sources} ${headers})
target_link_libraries(readCalibrationFormulaFile ${ROOT_LIBRARIES} Tracklib)

add_executable(GenerateMuonSinglePointCalibration test/GenerateMuonSinglePointCalibration.cpp ${sources} ${headers})
target_link_libraries(GenerateMuonSinglePointCalibration ${ROOT_LIBRARIES} Tracklib)

add_executable(GetEnergyCalibrationFromCosmicData test/GetEnergyCalibrationFromCosmicData.cpp ${sources} ${headers})
target_link_libraries(GetEnergyCalibrationFromCosmicData ${ROOT_LIBRARIES} Tracklib -lSpectrum)


add_executable(TestMuonEnergyCalib test/TestMuonEnergyCalib.cpp ${sources} ${headers})
target_link_libraries(TestMuonEnergyCalib ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis test/MuonTrackAnalysis.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_TestMuonTrackTimeEstimation test/MuonTrackAnalysis_TestMuonTrackTimeEstimation.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_TestMuonTrackTimeEstimation ${ROOT_LIBRARIES} Tracklib)

add_executable(TestMuonTrackTimeEstimation test/TestMuonTrackTimeEstimation.cpp ${sources} ${headers})
target_link_libraries(TestMuonTrackTimeEstimation ${ROOT_LIBRARIES} Tracklib)

add_executable(TestQMeanCorrected90Scintillators test/TestQMeanCorrected90Scintillators.cpp ${sources} ${headers})
target_link_libraries(TestQMeanCorrected90Scintillators ${ROOT_LIBRARIES} Tracklib)

add_executable(dissection test/dissection.cpp ${sources} ${headers})
target_link_libraries(dissection ${ROOT_LIBRARIES} Tracklib)

add_executable(dissectionCore test/dissectionCore.cpp ${sources} ${headers})
target_link_libraries(dissectionCore ${ROOT_LIBRARIES} Tracklib)

add_executable(RollOverDetection test/RollOverDetection.cpp ${sources} ${headers})
target_link_libraries(RollOverDetection ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_SimplestSetup test/MuonTrackAnalysis_SimplestSetup.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_SimplestSetup ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_SimplestSetup_Pixels test/MuonTrackAnalysis_SimplestSetup_Pixels.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_SimplestSetup_Pixels ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_SimplestSetup_Pixels_Compare test/MuonTrackAnalysis_SimplestSetup_Pixels_Compare.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_SimplestSetup_Pixels_Compare ${ROOT_LIBRARIES} Tracklib)


add_executable(Rollover_Inspection test/Rollover_Inspection.cpp ${sources} ${headers})
target_link_libraries(Rollover_Inspection ${ROOT_LIBRARIES} Tracklib)

add_executable(Rollover_Inspection2 test/Rollover_Inspection2.cpp ${sources} ${headers})
target_link_libraries(Rollover_Inspection2 ${ROOT_LIBRARIES} Tracklib)                 

add_executable(MuonTrackAnalysis_PixelCoincidence test/MuonTrackAnalysis_PixelCoincidence.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_PixelCoincidence ${ROOT_LIBRARIES} Tracklib)

add_executable(TestXYCenterForEachScintillator test/TestXYCenterForEachScintillator.cpp ${sources} ${headers})
target_link_libraries(TestXYCenterForEachScintillator ${ROOT_LIBRARIES} Tracklib)

add_executable(TestStripProfileOfLayersUsingHitInScintillators test/TestStripProfileOfLayersUsingHitInScintillators.cpp ${sources} ${headers})
target_link_libraries(TestStripProfileOfLayersUsingHitInScintillators ${ROOT_LIBRARIES} Tracklib)

add_executable(ReadCalibData test/ReadCalibData.cpp ${sources} ${headers})
target_link_libraries(ReadCalibData ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_StripProfileAnalysis test/MuonTrackAnalysis_StripProfileAnalysis.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_StripProfileAnalysis ${ROOT_LIBRARIES} Tracklib)


#add_executable(readCalibDataFile test/readCalibDataFile.cpp ${sources} ${headers})
#target_link_libraries(readCalibDataFile ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_UsingDelT test/MuonTrackAnalysis_UsingDelT.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_UsingDelT ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_UsingOnlyCenterBars test/MuonTrackAnalysis_UsingOnlyCenterBars.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_UsingOnlyCenterBars ${ROOT_LIBRARIES} Tracklib)

add_executable(BoardRates test/BoardRates.cpp ${sources} ${headers})
target_link_libraries(BoardRates ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_Anticoincidence test/MuonTrackAnalysis_Anticoincidence.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_Anticoincidence ${ROOT_LIBRARIES} Tracklib)


add_executable(MuonTrackAnalysis_CoinicidenceCountBetweenTopAndBottomLayer test/MuonTrackAnalysis_CoinicidenceCountBetweenTopAndBottomLayer.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_CoinicidenceCountBetweenTopAndBottomLayer ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_CoincidenceSimplified test/MuonTrackAnalysis_CoincidenceSimplified.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_CoincidenceSimplified ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_HitPointOnLayers test/MuonTrackAnalysis_HitPointOnLayers.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_HitPointOnLayers ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_HitPointOnLayers_Confined test/MuonTrackAnalysis_HitPointOnLayers_Confined.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_HitPointOnLayers_Confined ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_InspectionOfPixelWithScatterer test/MuonTrackAnalysis_InspectionOfPixelWithScatterer.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_InspectionOfPixelWithScatterer ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_ExtrapolationDiff test/MuonTrackAnalysis_ExtrapolationDiff.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_ExtrapolationDiff ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_RefinedTrackTest test/MuonTrackAnalysis_RefinedTrackTest.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_RefinedTrackTest ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_HitOnScattererPlane_UsingAntiCoincidenceScintillators test/MuonTrackAnalysis_HitOnScattererPlane_UsingAntiCoincidenceScintillators.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_HitOnScattererPlane_UsingAntiCoincidenceScintillators ${ROOT_LIBRARIES} Tracklib)

add_executable(CoreTest test/CoreTest.cpp ${sources} ${headers})
target_link_libraries(CoreTest ${ROOT_LIBRARIES} Tracklib)

add_executable(TestNewCalibFromDataFiles test/TestNewCalibFromDataFiles.cpp ${sources} ${headers})
target_link_libraries(TestNewCalibFromDataFiles ${ROOT_LIBRARIES} Tracklib)

add_executable(ReadCalibData2 test/ReadCalibData2.cpp ${sources} ${headers})
target_link_libraries(ReadCalibData2 ${ROOT_LIBRARIES} Tracklib)

add_executable(ReadCalibData3 test/ReadCalibData3.cpp ${sources} ${headers})
target_link_libraries(ReadCalibData3 ${ROOT_LIBRARIES} Tracklib)

add_executable(TestWritingVecOfStruct test/TestWritingVecOfStruct.cpp ${sources} ${headers})
target_link_libraries(TestWritingVecOfStruct ${ROOT_LIBRARIES} Tracklib)

add_executable(ReadNumOfEvents test/coreHelpers/ReadNumOfEvents.cpp ${sources} ${headers})
target_link_libraries(ReadNumOfEvents ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_CoreTest test/MuonTrackAnalysis_CoreTest.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_CoreTest ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_HitPointDisplacement test/MuonTrackAnalysis_HitPointDisplacement.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_HitPointDisplacement ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_SystemHealth test/MuonTrackAnalysis_SystemHealth.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_SystemHealth ${ROOT_LIBRARIES} Tracklib)

add_executable(PlotZenithAngle test/PlotZenithAngle.cpp ${sources} ${headers})
target_link_libraries(PlotZenithAngle ${ROOT_LIBRARIES} Tracklib)

add_executable(GenerateReverseParameterizationForSimulation test/GenerateReverseParameterizationForSimulation.cpp ${sources} ${headers})
target_link_libraries(GenerateReverseParameterizationForSimulation ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_ChargeCollectionRationVsDelT test/MuonTrackAnalysis_ChargeCollectionRationVsDelT.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_ChargeCollectionRationVsDelT ${ROOT_LIBRARIES} Tracklib)

add_executable(Param_Analysis test/Param_Analysis.cpp ${sources} ${headers})
target_link_libraries(Param_Analysis ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_GenerateDataFor_Classification test/MuonTrackAnalysis_GenerateDataFor_Classification.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_GenerateDataFor_Classification ${ROOT_LIBRARIES} Tracklib)
	
add_executable(MuonTrackAnalysis_SmearedHitPosition test/MuonTrackAnalysis_SmearedHitPosition.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_SmearedHitPosition ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_PlotMuonTracks test/MuonTrackAnalysis_PlotMuonTracks.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_PlotMuonTracks ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_Generate_ML_Data test/MuonTrackAnalysis_Generate_ML_Data.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_Generate_ML_Data ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_Generate_ML_Data_ForEachLayer test/MuonTrackAnalysis_Generate_ML_Data_ForEachLayer.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_Generate_ML_Data_ForEachLayer ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_Generate_ML_Data_Geometrical_Bounded test/MuonTrackAnalysis_Generate_ML_Data_Geometrical_Bounded.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_Generate_ML_Data_Geometrical_Bounded ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_FittedTracks_FromFile test/MuonTrackAnalysis_FittedTracks_FromFile.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_FittedTracks_FromFile ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_FittedTracks_FromFile2 test/MuonTrackAnalysis_FittedTracks_FromFile2.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_FittedTracks_FromFile2 ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_AnalyzeScint test/MuonTrackAnalysis_AnalyzeScint.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_AnalyzeScint ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_MuonClassficatonData test/MuonTrackAnalysis_MuonClassficatonData.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_MuonClassficatonData ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_MuonResolutionData test/MuonTrackAnalysis_MuonResolutionData.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_MuonResolutionData ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_ParameterizationUsingMuon test/MuonTrackAnalysis_ParameterizationUsingMuon.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_ParameterizationUsingMuon ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_TestMuonParam test/MuonTrackAnalysis_TestMuonParam.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_TestMuonParam ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_TestMuonParamDebug test/MuonTrackAnalysis_TestMuonParamDebug.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_TestMuonParamDebug ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_Generate_ML_Data_UsingCosmicMuons test/MuonTrackAnalysis_Generate_ML_Data_UsingCosmicMuons.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_Generate_ML_Data_UsingCosmicMuons ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_Dissection test/MuonTrackAnalysis_Dissection.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_Dissection ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_For_Chi2_Test test/MuonTrackAnalysis_For_Chi2_Test.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_For_Chi2_Test ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_HitMultiplicity test/MuonTrackAnalysis_HitMultiplicity.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_HitMultiplicity ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_HitMultiplicityDebug test/MuonTrackAnalysis_HitMultiplicityDebug.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_HitMultiplicityDebug ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_MuonLifeTime test/MuonTrackAnalysis_MuonLifeTime.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_MuonLifeTime ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_EnergySum test/MuonTrackAnalysis_EnergySum.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_EnergySum ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_MuonRate test/MuonTrackAnalysis_MuonRate.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_MuonRate ${ROOT_LIBRARIES} Tracklib)

add_executable(debugTest test/debugTest.cpp ${sources} ${headers})
target_link_libraries(debugTest ${ROOT_LIBRARIES} Tracklib)

add_executable(TestQMean_CalibAndUnCalib test/TestQMean_CalibAndUnCalib.cpp ${sources} ${headers})
target_link_libraries(TestQMean_CalibAndUnCalib ${ROOT_LIBRARIES} Tracklib)

add_executable(Test_BarInspection test/Test_BarInspection.cpp ${sources} ${headers})
target_link_libraries(Test_BarInspection ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_VisualizeTracks test/MuonTrackAnalysis_VisualizeTracks.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_VisualizeTracks ${root_libraries} tracklib visualizer)

add_executable(TestVisualizationFromROOTFile test/TestVisualizationFromROOTFile.cpp ${sources} ${headers})
target_link_libraries(TestVisualizationFromROOTFile ${ROOT_LIBRARIES} Tracklib Visualizer)

add_executable(MuonTrackAnalysis_AngularDistributionVerticalMuons test/MuonTrackAnalysis_AngularDistributionVerticalMuons.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_AngularDistributionVerticalMuons ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_LanGaus test/MuonTrackAnalysis_LanGaus.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_LanGaus ${ROOT_LIBRARIES} Tracklib)

add_executable(MuonTrackAnalysis_LanGausSingleBar test/MuonTrackAnalysis_LanGausSingleBar.cpp ${sources} ${headers})
target_link_libraries(MuonTrackAnalysis_LanGausSingleBar ${ROOT_LIBRARIES} Tracklib)

add_executable(Test_LanGausSampling test/Test_LanGausSampling.cpp ${sources} ${headers})
target_link_libraries(Test_LanGausSampling ${ROOT_LIBRARIES} Tracklib)

add_executable(Test_EnergyDepComparison test/Test_EnergyDepComparison.cpp ${sources} ${headers})
target_link_libraries(Test_EnergyDepComparison ${ROOT_LIBRARIES} Tracklib)

#add_executable(VisualizeTracksFromIsmran test/VisualizeTracksFromIsmran.cpp ${sources} ${headers})
#target_link_libraries(VisualizeTracksFromIsmran ${ROOT_LIBRARIES} Tracklib)

