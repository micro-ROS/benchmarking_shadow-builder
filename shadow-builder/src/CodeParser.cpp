#include <shadow-builder/CodeParser.h>

#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include <algorithm>

#if 0
Status SourceDir::excludeFile(std::string& pathFile, bool isRecursive)
{
	//TODO:IMPEMENT
	return Status::returnStatusOkay();
}

Status SourceDir::excludeFolder(std::string& pathFile, bool isRecursive)
{
	//TODO:IMPEMENT
	return Status::returnStatusOkay();
}

Status SourceDir::addFile(std::string& pathFile, bool isRecursive)
{
	//TODO:IMPEMENT
	return Status::returnStatusOkay();
}


Status SourceDir::retrieveListSourceFile(std::string& path, bool isRecursive)
{		
	if (access(path.c_str(), F_OK)) {
		/* Path does not exists */
		return Status::returnStatusError();	
	}

	if (auto dir = opendir(path.c_str())) {
		std::cout << "Current path in to look into: " << path << std::endl;
		while (auto f = readdir(dir)) {
			if (!f->d_name || f->d_name[0] == '.') continue;

			// TODO:MODERATE Remove filtre at this level, add a new filter
			// taken from the conmfiguration and the overriding of
			// the plugins.
			if (strstr(f->d_name, ".cpp")) {
				std::cout << f->d_name << std::endl;
			}	

			if (strstr(f->d_name, ".c")) {
				std::cout << f->d_name << std::endl;
			}	

			if (f->d_type == DT_DIR && isRecursive)  {
				std::string newPath = path + f->d_name + "/";
				retrieveListSourceFile(newPath, true);
			}
		}
		closedir(dir);

	}

	return Status::returnStatusOkay();
}

Status SourceDir::addFolder(std::string& pathFolder, bool isRecursive)
{
	std::vector<std::string>::iterator it;

	/* Look if path was already added or not */
	it = std::find(m_vecFolderPath.begin(), m_vecFolderPath.end(), pathFolder.c_str());
	if (it != m_vecFolderPath.end()) {
		return Status::returnStatusError();	
	}

	m_vecFolderPath.push_back(pathFolder);	
	if (retrieveListSourceFile(pathFolder, isRecursive)) {
		return Status::returnStatusError();	
	}

	return Status::returnStatusOkay();
}
#endif
