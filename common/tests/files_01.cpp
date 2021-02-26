#include  <common/FileSystem.h>
#include  <common/TestsConfig.h>


#include <iostream>
// #include <unistd.h>

using std::cout;
using std::endl;

int main(void)
{
	Files files;
	std::vector<File *> infos;
	FSElementFilterExt *filterEl = new FSElementFilterExt("cpp", false);
	files.addFilter(static_cast<FSElementFilter *>(filterEl));

	string test_folder = TEST_EX_SIMPLE_STUPID_EXAMPLE;
	files += test_folder;

	cout << " Opening folder with 'cpp' filter" << endl;
	infos = files.getSourceFiles();
	if(!infos.empty()) {
		cout << "Function getCurrentPath on 'cpp' filter failed" << endl;
		return -1; 
	}
	else {
		cout << "Files not found" << endl;
	}

	*filterEl = FSElementFilterExt("c", false);
	cout << " Opening folder with 'c' filter" << endl;
	infos = files.getSourceFiles();
	if(infos.empty()) {
		cout << "Function getCurrentPath on 'c' filter failed" << endl;
		return -1;
	}

	cout << "Found files:" << endl;
	for (File *file: infos) {
		cout << "\t" << file->getCurrentPath() << endl;
	}

	return 0;
}

