#include "Utility/ValueStore.h"

#include <stdlib.h>
#include <iostream>

int main(int argc, char** argv)
{
	dman::ValueStore store;
	auto a = store.Get<int>("a");
	auto a_clone = store.Get<int>("a");
	a.Initialize(new dman::SetValue<int>(10));
	std::cout << "a is " << a.GetValue() << std::endl;
	std::cout << "a_clone " << a_clone.GetValue() << std::endl;
	
	try
	{
		std::cout << "Attempting to reinitialize a..." << std::endl;
		a.Initialize(std::shared_ptr< const dman::Valuable<int> >());
	}
	catch(std::logic_error &le)
	{
		std::cout << le.what() << std::endl;
	}
	
	try
	{
		std::cout << "Attempting to reinitialize a_clone..." << std::endl;
		a_clone.Initialize(std::shared_ptr< const dman::Valuable<int> >());
	}
	catch(std::logic_error &le)
	{
		std::cout << le.what() << std::endl;
	}
	
	a.SetValue(4);
	std::cout << "a's value changed, checking a_clone: " << 
		a_clone.GetValue() << std::endl;
	
	try
	{
		std::cout << "Attempting to set a_clone..." << std::endl;
		a_clone.SetValue(10);
	}
	catch(std::logic_error &le)
	{
		std::cout << le.what() << std::endl;
	}
}