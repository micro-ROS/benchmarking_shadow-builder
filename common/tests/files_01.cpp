#include  <common/FileSystem.h>

#include <iostream>

using std::cout;
using std::endl;

int main(void)
{
	Files files;
	std::vector<File *> infos;
	FSElementFilterExt *filterEl = new FSElementFilterExt("cpp", false);
	files.addFilter(static_cast<FSElementFilter *>(filterEl));
	cout << "Opening folder" << endl;
	files += "/tmp/mytest/";

	infos = files.getSourceFiles();

	cout << "Found files:" << endl;
	for (File *file: infos) {
		cout << "\t" << file->getCurrentPath() << endl;
	}

	return 0;
}

