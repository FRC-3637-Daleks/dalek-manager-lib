#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <functional>

#include "Utility/FunkyPoll.h"
#include "Utility/TimeoutUpdater.h"

using namespace dman;
using namespace std::chrono_literals;

int main(int argc, char **argv)
{
	auto get_float = []() -> float
	{
		return 3.14;
	};

	FunkyPoll<float> float_poll(get_float);
	TimeoutWrapper wrapper(1s, 
		std::shared_ptr< FunkyPoll<float> >(&float_poll,
			 [](FunkyPoll<float> *){}));

	std::cout << "Before: " << float_poll.Get() << std::endl;

	wrapper.Update();

	std::cout << "After: " << float_poll.Get() << std::endl;

	return 0;
}