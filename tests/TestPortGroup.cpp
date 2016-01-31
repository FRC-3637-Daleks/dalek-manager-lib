#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

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
	root["CAN"]("cobra");
	root["CAN"]("lifter");
	root["CAN"]("lifter-slave");
	
	root["Solenoids"]["pistons"]("out").SetDefault(0);
	root["Solenoids"]["pistons"]("in").SetDefault(1);

	root["Solenoids"].PropogatePortSpace(std::make_shared<PortSpace>(0, 10));

	for(int i = 0; i < argc; i++)
	{
		if(string("load") == argv[i])
		{
			json input;
			
			ifstream file(argv[++i]);
			
			file >> input;
			try
			{
				if(root.LoadConfig(std::move(input)))
					cerr << "Config did not set all values" << endl;
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
		else if(string("assemble") == argv[i])
		{
			json input;
			
			ifstream file(argv[++i]);
			
			file >> input;
			try
			{
				if(root.AssembleConfig(std::move(input)))
					cerr << "Config did not set all values" << endl;
			}
			catch(const UnavailablePortError& upe)
			{
				cerr << upe.what() << endl;
			}
		}
	}
	
	return 0;
}
