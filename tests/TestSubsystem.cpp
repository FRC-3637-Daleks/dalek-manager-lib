#include <stdlib.h>
#include <iostream>

#include "Config/RootSystem.h"
#include "Config/PortSpace.h"

using namespace dman;

class Piston: public Subsystem
{
public:
	Piston(std::string id): Subsystem(id) {}

protected:
	void doRegister() override
	{
		auto& solenoids = GetPortSpace("Solenoids");
		solenoids("in");
		solenoids("out");

		auto& settings = GetSettings();
		settings("transition-seconds").SetDefault(1.0);
	}

	bool doConfigure() override
	{
		auto& solenoids = GetPortSpace("Solenoids");
		std::cout << get_name() + " Piston Solenoid in: " << 
			(sol_in = solenoids("in").GetValueOrDefault()) <<
			std::endl;

		std::cout << get_name() + " Piston Solenoid out: " <<
			(sol_out = solenoids("out").GetValueOrDefault()) <<
			std::endl;

		std::cout << get_name() + " Piston Transition time: " <<
			(transition = GetSettings()("transition-seconds").GetValueOrDefault()) <<
			std::endl;

		return true;
	}

private:
	int sol_in, sol_out;
	double transition;
};

class Robot: public RootSystem
{
public:
	Robot(std::string home_path):
		RootSystem(std::move(home_path)),
		left("left"), right("right")
	{
		AddSubSubsystem("left", left);
		AddSubSubsystem("right", right);
	}

private:
	Piston left, right;
};


int main(int argc, char **argv)
{
	if(argc < 2)
	{
		std::cout << "Usage: TestSubsystem <home-folder>" << std::endl;
		return -1;
	}



	std::cout << "Creating robot object" << std::endl;
	Robot robot(argv[1]);
	if(robot.get_context().has_no_manifest())
	{
		std::cout << "Construction failed" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "Robot object created" << std::endl;
	}

	robot.get_context().get_ports()["Solenoids"].PropogatePortSpace(
			std::make_shared<PortSpace>(0, 10));

	std::cout << "Registering..." << std::endl;
	robot.Register();
	std::cout << "Registered" << std::endl;

	std::cout << "Saving schema..." << std::endl;
	robot.get_context().SaveSchema();
	std::cout << "Schema saved" << std::endl;

	std::cout << "Configuring" << std::endl;
	if(robot.Configure())
		std::cout << "Robot configured successfully" << std::endl;
	else
		std::cout << "Robot is not ready" << std::endl;

	return 0;
}