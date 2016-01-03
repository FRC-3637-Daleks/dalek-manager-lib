#include <stdlib.h>
#include <thread>
#include <chrono>
#include <memory>
#include <iostream>

#include "Utility/FunkyPoll.h"
#include "Utility/TimeoutUpdater.h"

using namespace dman;
using namespace std::chrono_literals;

int main(int argc, char** argv)
{
	auto get_1 = []() -> int
	{
		std::cout << "Calculating number after 0";
		std::cout.flush();
		for(int i = 0; i < 3; i++)
		{
			std::cout << '.';
			std::cout.flush();
			std::this_thread::sleep_for(1s);
		}
		std::cout << " done!" << std::endl;
		return 1;
	};

	auto get_pi = []() -> float
	{
		std::cout << "Calculating number after 0";
		std::cout.flush();
		for(int i = 0; i < 5; i++)
		{
			std::cout << '.';
			std::cout.flush();
			std::this_thread::sleep_for(1s);
		}
		std::cout << " done!" << std::endl;
		return 3.1415;
	};

	auto three_second_poll = std::make_shared< FunkyPoll<int> >(
		std::move(get_1));

	auto five_second_poll = std::make_shared< FunkyPoll<float> >(
		std::move(get_pi));

	TimeoutWrapper wrapper(4s);

	wrapper.set_updater(three_second_poll);
	std::cout << "Running one second function..." << std::endl;
	if(wrapper.Update())
		std::cout << "Function timed out, as it should not have";
	else
		std::cout << "Function successfully calculated: " << 
			three_second_poll->Get() << std::endl;

	wrapper.set_updater(five_second_poll);
	std::cout << "Running two second function..." << std::endl;
	if(wrapper.Update())
	{
		std::cout << "Function timed out, as it should have." <<
			" Shouldn't have value: " << five_second_poll->Get() <<
			std::endl;

		std::this_thread::sleep_for(2s);
		std::cout << "The result should have calculated by now: " <<
			five_second_poll->Get() << std::endl;
	}
	else
		std::cout << "Function did not timeout, produced: " <<
			five_second_poll->Get() << std::endl;


}