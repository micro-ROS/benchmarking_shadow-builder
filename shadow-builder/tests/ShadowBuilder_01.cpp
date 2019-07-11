#include <shadow-builder/ShadowBuilder.h>
#include <iostream>

#define ARGV_0 (char*) "ShadowBuilder"
#define ARGV_1 (char*) "--cfg-shadowbuilder=/home/amalki/prjs/microROS/soft/apps/pipeline-benchmarking/res/sb-res/bcf_uros.xml"
#define ARGV_2 (char*) "--cfg-tfa=/home/amalki/prjs/microROS/soft/apps/pipeline-benchmarking/res/tfa-res/tfa.xml"

char *argv_[] = {ARGV_0, ARGV_1, ARGV_2};

int main(int argc, char **argv)
{
	ShadowBuilder sb(sizeof(argv_)/sizeof(argv_[0]), argv_);
	Status st;

	if ((st = sb.init())) {
		std::cout << "Shadowbuilder basic Initialization: [FAILED]" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << st << std::endl;
	std::cout << "Shadowbuilder basic Initialization: [OK]" << std::endl;

	if ((st = sb.startInstrumenting())) {
		exit(EXIT_FAILURE);
	}

	std::cout << st << std::endl;
	std::cout << "Shadowbuilder started shadow Building: [OK]" << std::endl;

	return 0;
}
