#include <iostream>
#include "Log/LogAttributes.h"

using namespace dman;

int main(int argc, char **argv)
{
	MessageData mess_data(MessageData::STATUS, 0);
	std::cout << "mess_data = " << mess_data << std::endl;
	
	SystemData sys_data("Tester", "main", "function");
	std::cout << "sys_data = " << sys_data << std::endl;
	
	SystemData sys_copy(sys_data);
	std::cout << "sys_copy = " << sys_copy << std::endl;
}