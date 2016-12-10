FLASCHEN_TASCHEN_API_DIR=ft/api
CXXFLAGS=-Wall -O3 -I$(FLASCHEN_TASCHEN_API_DIR)/include -I. -D__LINUX__
LDFLAGS=-L$(FLASCHEN_TASCHEN_API_DIR)/lib -lftclient 
WALABOT_LDFLAGS= -lWalabotAPI
FTLIB=$(FLASCHEN_TASCHEN_API_DIR)/lib/libftclient.a
YOUR_OBJECTS=...


all: getVersion ft-inwall-example ft-raw-example

getVersion: GetVersion.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(WALABOT_LDFLAGS)
ft-inwall-example: walabot-inwall-image-example.cpp 
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS) $(WALABOT_LDFLAGS)
ft-raw-example:    walabot-raw-image-example.cpp $(FTLIB)
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS) $(WALABOT_LDFLAGS)

$(FTLIB) :
	make -C $(FLASCHEN_TASCHEN_API_DIR)/lib


clean: 
	rm -f getversion ft-inwall-image-example ft-raw-example

