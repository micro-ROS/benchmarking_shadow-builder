#include <common/FileSystem.h>
#include <algorithm>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DEBUG 1

/** Predicate to verifty the type of filter */
static bool filterPredicateTypePartition(FSElementFilter *fs1) {
	return (dynamic_cast<FSElementFilterPath *>(fs1) != nullptr);
}


Files& Files::operator-(const std::string& exclude)
{
	m_Tree.excludePath(exclude);
	return *this;
}

Files& Files::operator+(const std::string& exclude)
{
	m_Tree.addPath(exclude, true);
	return *this;
}

Files& Files::operator-=(const std::string& exclude)
{
	return operator-(exclude);
}

Files& Files::operator+=(const std::string& addition) 
{
	return operator+(addition);
}

Status Files::addFilter(FSElementFilter* filter)
{
	m_filters.push_back(filter);

	return Status::returnStatusOkay();
}

std::vector<File *>& Files::getSourceFiles()
{
	m_Tree.searchFiles(m_filters);
	return m_Tree.getListOfFiles();
}

Folder::~Folder()
{
	for (FSElement *el: m_ChildElements) { 
		if (el != nullptr ) 
			delete el;
	}

	m_ChildElements.clear();
}

Status Folder::getFiles(
	std::vector<FSElement *>& filePaths
)
{
	filePaths.insert(filePaths.end(), m_ChildElements.begin(), m_ChildElements.end());
	return Status::returnStatusOkay();
}

Status TreeFS::addPath(const std::string& source, bool isRecursive)
{
	Status st;
	char real_path[PATH_MAX];
	struct stat sb;

	if (stat(source.c_str(), &sb)) {
		std::cout << "Error: Path does not exists" << std::endl;
		return Status::returnStatusError();	
	}
	if (stat(source.c_str(), &sb)) {
		std::cout << "Error: Path does not exists" << std::endl;
		return Status::returnStatusError();	
	}

	if (!S_ISDIR(sb.st_mode)) {
		m_TreeElements.push_back(new File(realpath(source.c_str(), 
							  real_path)));
	} else {
		m_TreeElements.push_back(new Folder(realpath(source.c_str(), real_path),
				       	isRecursive));
	}

	return st;
}

Status TreeFS::excludePath(const std::string& path)
{
	return Status::returnStatusOkay();
}

std::vector<File *>& TreeFS::getListOfFiles()
{
	return m_fileElements;
}

Status TreeFS::searchFiles(std::vector<FSElementFilter *>& filter)
{
	bool keep = false;
	bool hit = false;
	std::sort(filter.begin(), filter.end(), FSElementFilter::filterSorting);
	std::sort(m_TreeElements.begin(), m_TreeElements.end(),
		       	FSElement::elementSorting);

	std::unique(filter.begin(), filter.end(),
			FSElementFilter::filterpredicateUnique);

	std::unique(m_TreeElements.begin(), m_TreeElements.end(),
		    FSElement::elementpredicateUnique);

	/** Classify by exluded/nonexluded */
	std::partition(filter.begin(), filter.end(),
		    	FSElementFilter::filterPredicateExludeInclude);

	/** Classify by folder / extensin file */
	std::stable_partition(filter.begin(), filter.end(),
		    	filterPredicateTypePartition);

	for (FSElement *el : m_TreeElements) {
		Folder *dir = dynamic_cast<Folder *>(el);

		if (dir == nullptr) {
			continue;
		}

		m_searchSubFiles(el->getCurrentPath(), dir->getIsRecursive());
	}

	for (unsigned int i = 0; i < m_fileElements.size(); i++) {
		for (FSElementFilter* fse: filter) {
			std::string file = m_fileElements.at(i)->getCurrentPath();

			std::cout << "Checking filter" << fse->getFilterStr() << std::endl;
			if (fse->filterProcess(file, hit)) {
#ifdef DEBUG
				std::cout << "Removing element [" <<
					m_fileElements.at(i)->getCurrentPath() <<
				       "] because of " << fse->getFilterStr() << std::endl;
#endif
				delete m_fileElements.at(i);
				m_fileElements.erase(m_fileElements.begin() + i);
				i--;
				break;
			}

			if (hit) {
				break;
			}
		}
	}

	return Status::returnStatusOkay();
}


Status FileType::getFileType (
	std::string& extension,
	std::shared_ptr<FileType>& sharedFileType
)
{
	std::cout << "Extension" << std::endl;
	sharedFileType = std::make_shared<FileType>();

	return Status::returnStatusOkay();
}

bool TreeFS::m_isPart(FSElement& el1, FSElement& el2)
{
	if (dynamic_cast<File*>( &el1) !=  nullptr &&
		dynamic_cast<File*>( &el2) !=  nullptr) {

		return Status::returnStatusOkay();
	}

	std::size_t found = el1.getCurrentPath().find(el2.getCurrentPath());
	if (found != std::string::npos) {
		return true;
	}

	return false;
}

Status TreeFS::m_searchSubFiles(const std::string& path, const  bool isRecursive)
{
	Status st;
	DIR *dir = opendir(path.c_str());

#if DEBUG
	//TODO log file
	std::cout << "Current path in to look into: " << path << std::endl;
#endif

	if (!dir) {
		return Status::returnStatusError(std::string(
				"Invalid directory" + path));
	}

	while (struct dirent *f = readdir(dir)) {
		/** We do not care about hiden folders */
		if (!f->d_name || f->d_name[0] == '.') continue;

		std::string newPath = path + '/' + f->d_name;

#if DEBUG
		//TODO log file
		std::cout << "Found new path" << newPath << std::endl;
#endif
		FSElement *el = nullptr;

		if (f->d_type == DT_DIR && isRecursive)  {
			newPath += "/";

			st = m_searchSubFiles(newPath, isRecursive);
			if (st) { break; }

			//el = static_cast<FSElement *>(folder);
		} else {
			File *file = new File(newPath);

			if (file == nullptr) {
				return Status::returnStatusError("Could object file");
			}

			//el = static_cast<FSElement *>(file);
			m_fileElements.push_back(file);
		}
	}

	closedir(dir);
	return Status::returnStatusOkay();
}

bool FSElementFilterExt::filterProcess(const std::string& processedFile, bool& hit)
{
	if (processedFile.length() < m_filter.length()) {
		hit = false;
		return m_Excluded;
	}
	
	hit = ((processedFile.substr(processedFile.length() 
		- m_filter.length()) == m_filter));

	if (m_Excluded) {
		return hit;
	}

	return  !hit;
}

bool FSElementFilterPath::filterProcess(const std::string& processedFile, bool& hit)
{
	size_t pos;
	if (processedFile.length() < m_filter.length())
	{
		pos = m_filter.find(processedFile);
	} else {
		pos = processedFile.find(m_filter);
	}

	hit = (!m_Excluded && (pos != std::string::npos));

	return (m_Excluded && (pos != std::string::npos));
}
