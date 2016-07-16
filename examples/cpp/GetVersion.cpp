#include "WalabotAPI.h"
#include <iostream>
#include <stdio.h>
#include <string>

#define CHECK_WALABOT_RESULT(result, func_name)					\
{																\
	if (result != WALABOT_SUCCESS)								\
	{															\
		unsigned int extended = Walabot_GetExtendedError();		\
		const char* errorStr = Walabot_GetErrorString();		\
		std::cout << std::endl << "Error at " __FILE__ << ":"	\
                  << std::dec << __LINE__ << " - "				\
				  << func_name << " result is 0x" << std::hex	\
                  << result << std::endl;						\
																\
		std::cout << "Error string: " << errorStr << std::endl; \
																\
		std::cout << "Extended error: 0x" << std::hex			\
                  << extended << std::endl << std::endl;		\
																\
		std::cout << "Press enter to continue ...";				\
		std::string dummy;										\
		std::getline(std::cin, dummy);							\
		return;													\
	}															\
}


void GetWalabotVersion()
{
	// --------------------
	// Variable definitions
	// --------------------
	WALABOT_RESULT res;
	char* version;

	// Walabot_GetStatus - output parameters


	//	1) Connect : Establish communication with Walabot.
	//	==================================================
	res = Walabot_ConnectAny();
	CHECK_WALABOT_RESULT(res, "Walabot_ConnectAny");
	//      Print version
	res  = Walabot_GetVersion(&version);
	CHECK_WALABOT_RESULT(res, "Walabot_GetVersion");
	std::cout << version << std::endl; 
	//	======================

	res = Walabot_Disconnect();
	CHECK_WALABOT_RESULT(res, "Walabot_Disconnect");
}

#ifndef _SAMPLE_CODE_
int main()
{
	GetWalabotVersion();
}
#endif
