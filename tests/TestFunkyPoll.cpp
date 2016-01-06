#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <functional>
#include <string>

#include "Utility/FunkyPoll.h"
#include "Utility/TimeoutUpdater.h"

using namespace dman;
using namespace std::chrono_literals;

int main(int argc, char **argv)
{
	auto get_long_long = []() -> long long
	{
		return 1000000000000;
	};

	auto get_float = []() -> float
	{
		return 3.14;
	};

	auto get_double = []() -> double
	{
		return 0.99999999999;
	};

	auto get_string = []() -> std::string
	{
		return "hello, world";
	};

	FunkyPoll<long long> long_long_poll(get_long_long);
	FunkyPoll<float> float_poll(get_float);
	FunkyPoll<double> double_poll(get_double);
	FunkyPoll<std::string> string_poll(get_string);

	TimeoutWrapper wrapper(1s, 
		std::shared_ptr< FunkyPoll<long long> >(&long_long_poll,
			 [](FunkyPoll<long long> *){}));

	std::cout << "Before: " << long_long_poll.Get() << std::endl;
	wrapper.Update();
	std::cout << "After: " << long_long_poll.Get() << std::endl;

	wrapper.set_updater(std::shared_ptr< FunkyPoll<float> >(
		&float_poll, [](FunkyPoll<float> *){}));
	std::cout << "Before: " << float_poll.Get() << std::endl;
	wrapper.Update();
	std::cout << "After: " << float_poll.Get() << std::endl;

	wrapper.set_updater(std::shared_ptr< FunkyPoll<double> >(
		&double_poll, [](FunkyPoll<double> *){}));
	std::cout << "Before: " << double_poll.Get() << std::endl;
	wrapper.Update();
	std::cout << "After: " << double_poll.Get() << std::endl;

	wrapper.set_updater(std::shared_ptr< FunkyPoll<std::string> >(
		&string_poll, [](FunkyPoll<std::string> *){}));
	std::cout << "Before: " << string_poll.Get() << std::endl;
	wrapper.Update();
	std::cout << "After: " << string_poll.Get() << std::endl;

	return 0;
}