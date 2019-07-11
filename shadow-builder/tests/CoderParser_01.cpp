#include <shadow-builder/CodeParser.h>
#include <tfa_pr/TFACore.h>
#include <tfa/TFABenchMarkingPlatform.h>

#include <common/TestsConfig.h>
#include <common/FileSystem.h>
#include <common/BaseConfig.h>

#include <iostream>

int main(int argc, char ** argv)
{
	TreeFS tfs;
	Parser parser;
	std::vector<FSElementFilter *> filters =  { 
		new FSElementFilterPath(TEST_EX_SIMPLE_STUPID_EXAMPLE"/build/", true),
	       	new FSElementFilterExt(".c", false)
       	};

	unsigned int test_nbr = 0;
	string pathFolder = string(TEST_EX_SIMPLE_STUPID_EXAMPLE);

	std::cout << "Parsing folder: " << pathFolder << std::endl;
	
	if (tfs.addPath(pathFolder, true)) {
		std::cout << "Error Test: " << test_nbr << std::endl;
		return -1;
	}

	tfs.searchFiles(filters);
	TFABenchMarkingPlatform plat("Linux", "*", "*", "*");

	TFAConfiguration tfaConf(BASE_CFG_TFA_PLUGIN_DEFAULT_FOLDER,
				   plat);

	if (TFACore::initTFACore(tfaConf)) {
		return -1;
	}

	if (parser.setFilesToAnalyse(tfs.getListOfFiles())) {
		std::cout << "Could not set the path within the parser " << std::endl;
		return -1;
	}

	if (parser.startParsing()) {
		std::cout << "[] Failed" << std::endl;
		return -1;
	}

	std::cout << "[] Pass" << std::endl;
	return 0;
}
