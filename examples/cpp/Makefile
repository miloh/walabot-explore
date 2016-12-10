FLASCHEN_TASCHEN_API_DIR=ft/api
CXXFLAGS=-Wall -O2 -I$(FLASCHEN_TASCHEN_API_DIR)/include -I. -D__LINUX__

LDFLAGS=-L$(FLASCHEN_TASCHEN_API_DIR)/lib -lftclient
FTLIB=$(FLASCHEN_TASCHEN_API_DIR)/lib/libftclient.a

WALABOT_LDFLAGS= -lWalabotAPI

all: getVersion ft-inwall-example ft-raw-example

getVersion: GetVersion.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(WALABOT_LDFLAGS)
ft-inwall-example: walabot-inwall-image-example.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS) $(WALABOT_LDFLAGS)
ft-raw-example:    walabot-raw-image-example.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS) $(WALABOT_LDFLAGS)



clean: 
	rm -f getversion ft-inwall-image-example ft-raw-example

