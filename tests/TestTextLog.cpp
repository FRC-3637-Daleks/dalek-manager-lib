#include "Log/TextLog.h"
#include "Log/LogAttributes.h"

using namespace dman;

int main(int argc, char **argv)
{
	const SystemData sys_data("Tester", "main", "function");
	TextLog::Log(MessageData::INFO, sys_data) << "StreamHandle test successful";
	TextLog::Log(MessageData::STATUS, sys_data, "TextLog test successful!");
	
	auto strmA = TextLog::Log(MessageData::INFO, sys_data) << "This should flush ";
	auto strmB = TextLog::Log(MessageData::INFO, sys_data) << "This should flush ";
	
	(strmB << "first").Flush();
	strmA << "sec";
	strmA << "ond";
	
	return 0;
}
