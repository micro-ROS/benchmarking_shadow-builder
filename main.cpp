#include <shadow-builder/ShadowBuilder.h>
#include <cxxopts.hpp>
#include <iostream>

int main(int argc, char **argv)
{
	ShadowBuilder sb(argc, argv);
	Status st;

	if((st = sb.init())) {
		std::cout << "Shadow builder initialization failed" << std::endl;
		std::cout << "Reason: " << st << std::endl;
		exit(EXIT_FAILURE);
	}

	if (sb.startInstrumenting()) {
		std::cout << "Shadow builder instrumentation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	exit (EXIT_SUCCESS);
}
