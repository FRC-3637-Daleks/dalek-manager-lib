#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

#include "Config/SettingGroup.h"
#include "Config/UnavailablePortError.h"

using namespace dman;
using namespace std;

int main(int argc, char **argv)
{
	SettingGroup root;
	
	root["drive"]("precision").SetDefault(0.5);
	{
		auto& limits = root["cobra"]["soft_limits"];
		limits("bottom").SetDefault(10);
		limits("top").SetDefault(600);
	}

	root["auton"]("mode").SetDefault("portcullis");

	for(int i = 0; i < argc; i++)
	{
		if(string("load") == argv[i])
		{
			json input;
			
			ifstream file(argv[++i]);
			
			file >> input;
			try
			{
				if(root.LoadConfig(input))
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
				if(root.AssembleConfig(input))
					cerr << "Config could not generate all values" << endl;
			}
			catch(const UnavailablePortError& upe)
			{
				cerr << upe.what() << endl;
			}
		}
	}
	
	return 0;
}
