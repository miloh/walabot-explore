CXXFLAGS=-Wall -O2 -D__LINUX__
WALABOT_LDFLAGS= -lWalabotAPI

all: sensorTarget sensorBreathing inWall


sensorTarget: SensorCodeSample.cpp
	$(CXX) -o $@.out $< $(CXXFLAGS) $(WALABOT_LDFLAGS)
sensorBreathing: SensorBreathingSampleCode.cpp
	$(CXX) -o $@.out $< $(CXXFLAGS) $(WALABOT_LDFLAGS)
inWall: InWallSampleCode.cpp
	$(CXX) -o $@.out $< $(CXXFLAGS) $(WALABOT_LDFLAGS)


clean: 
	rm -f sensorTarget.out sensorBreathing.out inWall.out





