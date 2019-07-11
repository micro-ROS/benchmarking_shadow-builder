#include <common/Status.h>
#include <iostream>

int main(int argc, char **argv)
{
	Status st;

	std::cout << st << std::endl;

	st = Status::ERROR;

	if (!(st == Status::ERROR)) {
		std::cout << "Operator equal type failed" << std::endl;
		return -1;
	}

	if (st != Status::ERROR) {
		std::cout << "Operator equal type failed" << std::endl;
		return -1;
	}

	if (st != -1) {
		std::cout << "Operator equal type failed" << std::endl;
		return -1;
	}

	st = string("Test2");
	if (!(st == string("Test2"))) {
		std::cout << "Operator equal type failed" << std::endl;
		return -1;
	}

	return 0;
}


