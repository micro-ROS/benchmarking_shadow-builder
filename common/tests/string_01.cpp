#include <common/StringMan.h>
#include <iostream>

using namespace common;

int main(int argc, char **argv)
{
	std::vector<std::string> strings;
	std::string str = "/** Benchmarking::TimeBenchmarking::Timer */";

	std::cout << "Checking trimSpace:" << std::endl;
	std::cout << "Length of string is -> " << str.length() << std::endl;
	StringMan::trimSpace(str);
	if (str.find(" ") != std::string::npos)	{
		std::cout << "Function trimSpace failed" << std::endl;
		return -1;
	}
	std::cout << "OK" << std::endl;

	std::cout << "Checking removeCommentBeacons:" << std::endl;
	StringMan::removeCommentBeacons(str);
	if ((str.find("/**") != std::string::npos) || (str.find("*/") != std::string::npos))	{
		std::cout << "Function removeCommentBeacons failed" << std::endl;
		return -1;
	}
	std::cout << "OK" << std::endl;

	std::cout << "Checking split:" << std::endl;
	if (!StringMan::split(str, "::", strings))	{
		std::cout << "Function split failed" << std::endl;
		return -1;
	}
	for(std::string s : strings) {
		std::cout << s << std::endl;
	}
	if ((strings.size() != 3) || 
			strings[0].compare("Benchmarking") ||
			strings[1].compare("TimeBenchmarking") ||
			strings[2].compare("Timer")) {
		std::cout << "Function split failed" << std::endl;
		return -1;
	
	}
	std::cout << "OK" << std::endl;

	return 0;
}


