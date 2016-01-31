#include <stdlib.h>
#include <string>
#include <iostream>

#include "Config/PortGroup.h"
#include "Config/PortSpace.h"
#include "Config/UnavailablePortError.h"

using namespace dman;
using namespace std;

int main(int argc, char **argv)
{
	PortGroup root;
		
	root["CAN"].SetPortSpace(std::make_shared<PortSpace>(0, 63));
	
	{
		auto& drive = root["CAN"]["drive"];
		drive("front-left");
		drive("front-right");
		drive("rear-left");
		drive("rear-right");
	}
	
	root["CAN"]("PDP");
	root["CAN"]("PCM");
	
	root["Solenoids"]["pistons"]("out").SetDefault(0);
	root["Solenoids"]["pistons"]("in").SetDefault(1);

	root["Solenoids"].PropogatePortSpace(std::make_shared<PortSpace>(0, 10));

	for(int i = 0; i < argc; i++)
	{
		if(string("load") == argv[i])
		{
			json input;
			cin >> input;
			try
			{
				root.LoadConfig(input);
			}
			catch(const UnavailablePortError& upe)
			{
				cerr << upe.what() << endl;
			}
		}
		else if(string("get") == argv[i])
		{
			cout << std::setw(4) << root.GetConfig() << endl;
		}
		else if(string("schema") == argv[i])
		{
			cout << std::setw(4) << root.GetSchema() << endl;
		}
	}
	
	return 0;
}
