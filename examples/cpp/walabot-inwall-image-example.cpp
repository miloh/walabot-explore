#include "udp-flaschen-taschen.h"
#include <unistd.h>
#include <WalabotAPI.h>
#include <iostream>
#include <stdio.h>
#include <string>

//using namespace std;
using std::string;
using std::getline;
using std::cout;
using std::endl;
using std::cin;
using std::hex;
using std::dec;


#define DISPLAY_WIDTH  45
#define DISPLAY_HEIGHT 35

#define CHECK_WALABOT_RESULT(result, func_name)					\
{										\
	if (result != WALABOT_SUCCESS)						\
	{									\
		unsigned int extended = Walabot_GetExtendedError();		\
		const char* errorStr = Walabot_GetErrorString();		\
		cout << endl << "Error at " __FILE__ << ":"			\
                  << dec << __LINE__ << " - "					\
				  << func_name << " result is 0x" << hex	\
                  << result << endl;						\
										\
		cout << "Error string: " << errorStr << endl; 			\
										\
		cout << "Extended error: 0x" << hex				\
                  << extended << endl << endl;					\
										\
		cout << "Press enter to continue ...";				\
		string dummy;							\
		getline(cin, dummy);						\
		return;								\
	}									\
}

void PrintSensorImage(int* rasterImage,int sizeX,int sizeY,double sliceDepth,double power)
{
//
//    	#ifdef __LINUX__
//		printf("\033[2J\033[1;1H");
//	#else
//	    system("cls");
//	#endif
//
//	cout << "first image" << endl;
//	cout << "this array is: " << rasterImage << endl;
//	cout << "this sizeof rasterimage: " << sizeof(rasterImage) << endl;
//	cout << "sizeX: " << sizeX << endl;
//	cout << "sizeY: " << sizeY << endl;
//	cout << "sliceDepth: " << sliceDepth << endl;
//	int *current_value = rasterImage;
//        // Open socket and create our canvas.
//        const int socket = OpenFlaschenTaschenSocket(hostname);
//        UDPFlaschenTaschen canvas(socket, DISPLAY_WIDTH, DISPLAY_HEIGHT);
//	for (int y = 0; y < sizeY; ++y) {
//	  	for (int x = 0; x < sizeX; ++x) {
//    		    canvas.SetPixel(x, y, );              // Sample with color variable.
//    		    UDPFlaschenTaschen canvas(socket, DISPLAY_WIDTH, DISPLAY_HEIGHT);
//		     //fprintf(stderr, "%02x ", *current_value);
//		     ++current_value;
//		}
//                canvas.Send();                           // Send the framebuffer.
//		fprintf(stderr, "\n");
//	}
//
//unsigned char* imgdata = ...;
//size_t         imgdata_size = ...;
//
//std::fstream imgout("MyImage.bin", std::out | std::binary);
//
//imgout.write(reinterpret_cast<char*>(imgdata), imgdata_size);
//imgout.close();
}

void PrintSensorTargets(SensorTarget* targets, int numTargets)
{
	int targetIdx;

    #ifdef __LINUX__
		printf("\033[2J\033[1;1H");
	#else
	    system("cls");
	#endif

	if (numTargets > 0)
	{
		for (targetIdx = 0; targetIdx < numTargets; targetIdx++)
		{
			printf("Target #%d: \nX = %lf \nY = %lf \nZ = %lf \namplitude = %lf\n\n\n ",
				targetIdx,
				targets[targetIdx].xPosCm,
				targets[targetIdx].yPosCm,
				targets[targetIdx].zPosCm,
				targets[targetIdx].amplitude);
		}
	}
	else
	{
		printf("No target detected\n");
	}
}

void SensorCode_SampleCode(const char* hostname)
{
	// --------------------
	// Variable definitions
	// --------------------
	WALABOT_RESULT res;

	// Walabot_GetSensorTargets - output parameters
	SensorTarget* targets;
	int numTargets;

	// Walabot_GetStatus - output parameters
	APP_STATUS appStatus;
	double calibrationProcess; // Percentage of calibration completed, if status is STATUS_CALIBRATING

   // Walabot_GetRawImageSlice - output parameters
	int*	rasterImage = NULL;
	int		sizeX = -1;
	int		sizeY = -1;
	double	sliceDepth;
	double	power;

	// ------------------------
	// Initialize configuration
	// ------------------------

	double zArenaMin = 2;
	double zArenaMax = 12;
	double zArenaRes = 0.5;

	double xArenaMin = -6;
	double xArenaMax = 6;
	double xArenaRes = 0.5;

	double yArenaMin = -8;
	double yArenaMax = 8;
	double yArenaRes = 0.5;

	// ----------------------
	// Sample Code Start Here
	// ----------------------

	/*
		For an image to be received by the application, the following need to happen :
		1) Connect
		2) Configure
		3) Calibrate
		4) Start
		5) Trigger
		6) Get action
		7) Stop/Disconnect
	*/

	bool mtiMode = false;

	// Configure Walabot database install location
	#ifdef __LINUX__
	    res = Walabot_SetSettingsFolder((char*)"/var/lib/walabot");
	#else
	    res = Walabot_SetSettingsFolder("C:/ProgramData/Walabot/WalabotSDK");
	#endif
	
	CHECK_WALABOT_RESULT(res, "Walabot_SetSettingsFolder");	

	//	1) Connect : Establish communication with Walabot.
	//	==================================================
	res = Walabot_ConnectAny();
	CHECK_WALABOT_RESULT(res, "Walabot_ConnectAny");

	//  2) Configure : Set scan profile and arena
	//	=========================================

	// Set Profile - to Sensor. 
	//			Walabot recording mode is configure with the following attributes:
	//			-> Distance scanning through air; 
	//			-> high-resolution images
	//			-> slower capture rate 
	// nope -- set profile to Short Range Imaging!!
	res = Walabot_SetProfile(PROF_SHORT_RANGE_IMAGING);
	CHECK_WALABOT_RESULT(res, "Walabot_SetProfile");

	// Set arena by Cartesian coordinates, with arena resolution :
	res = Walabot_SetArenaX(xArenaMin, xArenaMax, xArenaRes);
	CHECK_WALABOT_RESULT(res, "Walabot_SetArenaX");

	res = Walabot_SetArenaY(yArenaMin, yArenaMax, yArenaRes);
	CHECK_WALABOT_RESULT(res, "Walabot_SetArenaY");

	res = Walabot_SetArenaZ(zArenaMin, zArenaMax, zArenaRes);
	CHECK_WALABOT_RESULT(res, "Walabot_SetArenaZ");


	FILTER_TYPE filterType = mtiMode ?
		FILTER_TYPE_MTI :		//Moving Target Identification: standard dynamic-imaging filter
		FILTER_TYPE_NONE;

	res = Walabot_SetDynamicImageFilter(filterType);
	CHECK_WALABOT_RESULT(res, "Walabot_SetDynamicImageFilter");

	//	3) Start: Start the system in preparation for scanning.
	//	=======================================================
	res = Walabot_Start();
	CHECK_WALABOT_RESULT(res, "Walabot_Start");

	//	4) Trigger: Scan(sense) according to profile and record signals to be available
	//	for processing and retrieval.
	//	================================================================================
	if (!mtiMode) // if MTI mode is not set - start calibrartion
	{
		// calibrates scanning to ignore or reduce the signals
		res = Walabot_StartCalibration();
		CHECK_WALABOT_RESULT(res, "Walabot_StartCalibration");
	}

	bool recording = true;

	while (recording)
	{
		// calibrates scanning to ignore or reduce the signals
		res = Walabot_GetStatus(&appStatus, &calibrationProcess);
		CHECK_WALABOT_RESULT(res, "Walabot_GetStatus");

		//	5) Trigger: Scan(sense) according to profile and record signals to be 
		//	available for processing and retrieval.
		//	====================================================================
		res = Walabot_Trigger();
		CHECK_WALABOT_RESULT(res, "Walabot_Trigger");

		//	6) 	Get action : retrieve the last completed triggered recording 
		//	================================================================
		res = Walabot_GetSensorTargets(&targets, &numTargets);
		CHECK_WALABOT_RESULT(res, "Walabot_GetSensorTargets");

		res = Walabot_GetRawImageSlice(&rasterImage, &sizeX, &sizeY, &sliceDepth, &power);
		CHECK_WALABOT_RESULT(res, "Walabot_GetRawImageSlice");
		std::cout << "Size : " << sizeX << "x" << sizeY << std::endl;

		//	******************************
		//	TODO: add processing code here
		//	******************************
		// add function to print the data from a single image.
	//	PrintSensorImage(rasterImage, sizeX, sizeY, sliceDepth, power);
    	#ifdef __LINUX__
		printf("\033[2J\033[1;1H");
	#else
	    system("cls");
	#endif

	printf("First Image\n");
	cout << "sizeX: " << sizeX << endl;
	cout << "sizeY: " << sizeY << endl;
	cout << "sliceDepth: " << sliceDepth << endl;
	int *current_value = rasterImage;
        // Open socket and create our canvas.
        const int socket = OpenFlaschenTaschenSocket(hostname);
        UDPFlaschenTaschen canvas(socket, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	for (int y = 0; y < sizeY; ++y) {
	  	for (int x = 0; x < sizeX; ++x) {
    		    canvas.SetPixel(x/2, y, Color(0,*current_value,0));         // Sample with color variable.
		     //fprintf(stderr, "%02x ", *current_value);
		     ++current_value;
		}
		//usleep(5000);
                //canvas.Send();                           // Send the framebuffer.
		//fprintf(stderr, "\n");
	}
        canvas.Send();
		//recording = false;

		// PrintSensorTargets(targets, numTargets);
	}

	//	7) Stop and Disconnect.
	//	======================
	res = Walabot_Stop();
	CHECK_WALABOT_RESULT(res, "Walabot_Stop");

	res = Walabot_Disconnect();
	CHECK_WALABOT_RESULT(res, "Walabot_Disconnect");
}


#ifndef _SAMPLE_CODE_
int main(int argc, char *argv[])
{
    const char *hostname = NULL;   // Will use default if not set otherwise.
    if (argc > 1) {
        hostname = argv[1];        // Hostname can be supplied as first arg
    }
    SensorCode_SampleCode(hostname);
}
#endif
