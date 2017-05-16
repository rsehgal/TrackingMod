name=daq904
helpname=baseline

#CAENVME_LIB=/usr/lib #/opt/xdaq/lib
#CAENVME_INC=/usr/local/include

CAENVME_LIB=/opt/lib
CAENVME_INC=/opt/include


ROOT_LIB=$(shell root-config --libs)
ROOT_INC=$(shell root-config --cflags)

CXX=g++

CXXFLAGS=-Wall -O0 -ggdb -I ../
CPPFLAGS=
LDFLAGS= -ggdb -lpthread -ldl

ifdef debug
  CPPFLAGS+=-DDEBUG_=1
endif

LDFLAGS+= -L$(CAENVME_LIB) -lCAENVME $(ROOT_LIB)
CPPFLAGS+= -I$(CAENVME_INC) $(ROOT_INC)

.SUFFIXES:
.SUFFIXES: .C .cc .o


files   := $(wildcard *.cpp)
headers := $(wildcard *.h)
objects := $(patsubst %.cpp,%.o,$(files))
objects += adc_v1721_caen.o daq904.o

$(name) : $(objects) #$(headers) 
	#@echo $(objects)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $(name) $(objects)

daq904.o: daq904.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c daq904.cc

adc_v1721_caen.o: adc_v1721_caen.cc adc_v1721_caen.hh
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c adc_v1721_caen.cc

#BSCModule.o : BSCModule.h
$%.o : $%.cpp $%.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

BSCDataReader.o : BSCDataReader.cpp BSCDataReader.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

ModuleVmeInterface.o : ModuleVmeInterface.cpp ModuleVmeInterface.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@


ModuleV2718.o : ModuleV2718.cpp ModuleV2718.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

ModuleV1190.o : ModuleV1190.cpp ModuleV1190.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

ModuleV1190Data.o : ModuleV1190Data.cpp ModuleV1190Data.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

BSCModuleV1721.o : BSCModuleV1721.cpp BSCModuleV1721.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

BSCModule.o : BSCModule.cpp BSCModule.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

IniFile.o : IniFile.cpp IniFile.h
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@


helpfiles   := $(wildcard *.C)
helpheaders := $(wildcard *.H)
helpobjects := $(patsubst %.C,%.O,$(helpfiles)) adc_v1721_caen.o

$(helpname): $(helpobjects)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $(helpname) $(helpobjects)

baseline.O: ../baseline.C ../generic_board.H adc_v1721_caen.hh
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c baseline.C -o baseline.O

generic_board.O: generic_board.C generic_board.H
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c generic_board.C -o generic_board.O

#adc_v1721_caen.o: adc_v1721_caen.cc adc_v1721_caen.hh
#	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c adc_v1721_caen.cc



#ana : analyse_file
#
#analyse_file: analyse_file.cxx
#	g++ -o analyse_file `root-config --cflags --glibs` -lThread analyse_file.cxx 


.PHONY: bkp cleanall clean

bkp :   $(headers) $(files) $(helpheaders) $(helpfiles) run_bcm1f.sh
	@tar -czf V1721_selftrigger_$(shell date +%Y-%b-%d).tgz $(headers) $(files) $(helpheaders) $(helpfiles) Makefile run_bcm1f.sh configure.sh show_Event.cxx

cleanall : clean
	@rm -f *.O $(helpname)
	
clean :
	@rm -f *.o $(name) *~
