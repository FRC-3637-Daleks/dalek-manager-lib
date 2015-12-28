#include "Log/TextLog.h"
#include "Log/LogAttributes.h"

using namespace dman;

int main(int argc, char **argv)
{
	TextLog::Log(MessageData::INFO, SystemData("Tester", "", "")) << "StreamHandle test successful";
	TextLog::Log(MessageData::STATUS, SystemData("Tester", "", ""), "TextLog test successful!");
	return 0;
}
