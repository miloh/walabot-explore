Walabot examples for Linux 
==========================

Install the Walabot .deb for Linux

Build the files in the example directory with make 

The existing C++ sample files work with the Walabot's 3 basic scan profiles.  The following is quoted from the Walabot API
  * Short range: Short range penetrative scanning inside dielectric materials such as wall.
  * Sensor: High resolutino images with a slower capture rate.
  * Sensor Narrow:  Lower resolution images for a fast capture rate. Useful for tracking quick movement.

```
host$ grep -ir PROF_ ./*
./InWallSampleCode.cpp:	res = Walabot_SetProfile(PROF_SHORT_RANGE_IMAGING);
./SensorBreathingSampleCode.cpp:	res = Walabot_SetProfile(PROF_SENSOR_NARROW);
./SensorCodeSample.cpp:	res = Walabot_SetProfile(PROF_SENSOR);
```
