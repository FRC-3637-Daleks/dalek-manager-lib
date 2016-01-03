#include <stdlib.h>
#include "Utility/UpdateThread.h"
#include "Utility/UpdateStore.h"
#include "Utility/FunkyPoll.h"

#include <chrono>
#include <iostream>
#include <string>

using namespace dman;
using namespace std::chrono_literals;

int main(int argc, char **argv)
{
	using clock_t = std::chrono::steady_clock;

	auto start = clock_t::now();

	auto up_seconds = [start = start]() -> int
	{
		return std::chrono::duration_cast<std::chrono::seconds>(
			clock_t::now() - start).count();
	};

	auto up_milliseconds = [start = start]() -> long
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(
			clock_t::now() - start).count();
	};

	FunkyPoll<int> poll_seconds(up_seconds);
	FunkyPoll<long> poll_milliseconds(up_milliseconds);

	UpdateStore store(20ms);
	store.Add(&poll_seconds);
	store.Add(&poll_milliseconds);

	UpdateThread thread(50ms, 
		std::make_shared<UpdateStore>(std::move(store)));

	thread.Start();

	std::string buffer;
	while(buffer != "exit")
	{
		std::cin >> buffer;
		if(buffer == "seconds")
			std::cout << poll_seconds.Get() << std::endl;
		else if(buffer == "milliseconds")
			std::cout << poll_milliseconds.Get() << std::endl;
	}
}