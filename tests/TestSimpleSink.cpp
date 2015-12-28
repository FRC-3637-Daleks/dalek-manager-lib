#include "Log/TextLog.h"
#include "Log/LogAttributes.h"

#include <string>

using namespace dman;

int main(int argc, char **argv)
{
	const SystemData sys_data("Tester", "main", "function");

	auto my_sink_fn = [] (std::string str)
	{
		std::cout << str << " {Brought to you by cout}" << std::endl;
	};

	TextLog::AddSimpleSink(my_sink_fn);

	TextLog::Log(MessageData::STATUS, sys_data, "SimpleSink test successful!");
	
	return 0;
}
