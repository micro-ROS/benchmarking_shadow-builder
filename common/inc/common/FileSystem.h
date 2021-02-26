#ifndef __COMMON_FILESYSTEM_H__
#define __COMMON_FILESYSTEM_H__

#include <unordered_map> 
#include <common/Status.h>
#include <string>
#include <vector>
#include <memory>

class FileSystem;
class Files;

class FileType {
public:
	/** \brief List of file type */
	typedef enum {
		/** *.cpp file type */
		FileType_CPP,
		/** *.c file type */
		FileType_C,
		/** *.h file type */
		FileType_H,
		/** *.hpp file type */
		FileType_HPP,
		/** SO library shared object */
		FileType_SO,
		/** Makefile file type */
		FileType_Makefile,
		/** CMakeList.txt file type */
		FileType_CMakeLists,
	} FileExtension;

	static Status getFileType(std::string& extension, 
			std::shared_ptr<FileType>& sharedFileType );
private:
	/** This is a list of extesion that categorise the output */
#if 0
	static struct TYPEFile {
		const std::vector<std::string> extension;
	} m_Types[] = {
		[FileType_CPP] = {
			.extension = {".cpp"}
		},
	        [FileType_C] = {
			.extension = {".c"}
		},
	        [FileType_H] = {
			.extension = {".h"}
		},
	        [FileType_HPP] = {
			.extension = {".hpp"}
		},
	        [FileType_SO] = {
			.extension = {".so"}
		},
	       	[FileType_Makefile] = {
			.extension = {"Makefile"}
		},
	       	[FileType_CMakeLists] = {
			.extension = {"CMakeLists.txt"}
		}
	};
#endif
};

/**
 * \brief This is the File system base class holding information regarding 
 * 		the FS node path
 */
class FSElement {
public:
	/** 
	 * \brief Ctor: Need to be used no implicit conversion 
	 * \param path: Path of the FS node.
	 **/
	 explicit FSElement(const string& path): m_ElementPath(path) {}
	 virtual ~FSElement() {}

	/**
	 * \brief Retrieve the current file path of the FS node.
	 * \return a reference on the FS's node path string. 
	 */
	inline const std::string& getCurrentPath() const { return m_ElementPath;}

	/**
	 * \brief This function is needed to sort the element for later remval
	 */
	static bool elementSorting(const FSElement* el1, const FSElement* el2)
	{
		return (el1->m_ElementPath < el2->m_ElementPath);
	}

	static bool elementpredicateUnique(const FSElement* el1, const FSElement* el2)
	{
		return (el1->m_ElementPath < el2->m_ElementPath);
	}

protected:
	/** Path to the to the FS node */
	const std::string m_ElementPath;
};


class FSElementFilter {
public:
	FSElementFilter(const std::string& filter, bool isExcluded):
	m_filter(filter), m_Excluded(isExcluded) {}

	/**
	 * \brief This function is need for some cleanups
	 * \brief object1 to compare.
	 * \brief object2 to compare.
	 * \return if object1 < to object3
	 */
	static bool filterSorting(FSElementFilter* fs1, FSElementFilter* fs2)
	{
		return (fs1->m_filter < fs2->m_filter);
	}

	static bool filterpredicateUnique(FSElementFilter* fs1, FSElementFilter* fs2)
	{
 		bool isNotUnique = (fs1->m_filter == fs2->m_filter);
		if (isNotUnique && (fs1->m_Excluded != fs2->m_Excluded)) 
		{
			std::cout << "Warning same filter " << 
				fs1->m_filter <<
				" applying opposite rule!" << std::endl;
		}

		return isNotUnique;
	}

	static bool filterPredicateExludeInclude(const FSElementFilter *fs1) {
		return fs1->m_Excluded;
	}

	/**
	 * \brief This function provide a way to check if the file is to be
	 * 	added  to the lookup path or not.
	 * \param string processedFile: Reference on a string where the data
	 * 		is tot be modified. 
	 * TODO Conflict;
	 */
	virtual bool filterProcess(const std::string& processedFile, bool& hit) = 0;

	std::string& getFilterStr() { return m_filter;}
protected:
	std::string m_filter;
	bool m_Excluded;
};


class FSElementFilterExt : public FSElementFilter {
public:
	FSElementFilterExt(const std::string& filter, bool isExcluded):
	       	FSElementFilter(filter, isExcluded) {}

	/**
	 * \brief implementation
	 */
	virtual bool filterProcess(const std::string& processedFile, bool& hit);

};


class FSElementFilterPath : public FSElementFilter {
public:
	FSElementFilterPath(const std::string& filter, bool isExcluded):
	       	FSElementFilter(filter, isExcluded) {} 

	/**
	 * \brief implementation of the virtual function
	 */
	virtual bool filterProcess(const std::string& processedFile, bool& hit );
};


/**
 * \brief This is the FSElement implementation.
 *	This class is representing a File object.
 * \seealso class FSElement
 */
class File: public FSElement {
public:
	/**
	 * \brief Create an File object with its path and type of file
	 */
	explicit File(const std::string& path): FSElement(path) {}

	/**
	 * \brief return the file type.
	 */
	inline FileType getFileType() const { return m_FileType; }

	/**
	 * \brief Set the file type.
	 * \param filetype:  
	 */
	inline void setFileType(const FileType filetype) const 
				{ m_FileType = filetype ; }

private:
	/** This is the Filetype of the curren path node file */
	mutable FileType m_FileType;
};

/**
 * \brief This is the FSElement implementation.
 *	This class is representing a Folder object.
 * \seealso class FSElement
 */
class Folder: public FSElement {
public:
	/** 
	 * \brief Ctor creating a folder object. By default the object
	 * 		will recursively look for files.
	 * \param path: path to the current folder
	 * \param isRecursive: is the research of a file in a folder should be 
	 * 			or not. default value is false;
	 */
	explicit Folder(const std::string& path, bool isRecursive = false):
	       	FSElement(path), m_isRecursive(isRecursive) {}

	/* DTor cleaning the child references */
	~Folder();

	Status addChild(FSElement& element)
	{ 
		m_ChildElements.push_back(&element);
       		return Status::returnStatusOkay();
	}

	/**
	 * \brief This function retrieves the path to all the file and subfile
	 * \param filePaths: This is vector holding all the File path in this
	 * the folder. 
	 */
	Status getFiles(
		std::vector<FSElement *>& filePaths);

	bool getIsRecursive() const { return m_isRecursive; }

private:
	/** List of File in the folder */
	std::vector<FSElement *> m_ChildElements;
	bool m_isRecursive;
};


/** This is the File system tree that hold all file/folder */
class TreeFS { 
public:
	Status excludePath(const std::string& path);
	Status addPath(const std::string& source, bool isRecursive = false);

	Status searchFiles(std::vector<FSElementFilter *>& filter);
	std::vector<File *>& getListOfFiles();

private:
	Status m_searchSubFiles(const std::string& path, bool isRecursive);
	bool m_isPart(FSElement& el1, FSElement& el2);
	Status m_cleanTreeElements();

	Status m_excludeFile(std::string& path);
	Status m_excludeFolder(std::string& path);

	Status m_addFile(std::string& path);
	Status m_addFolder(std::string& path, bool isRecursive);

	mutable shared_ptr<std::vector<std::string>> m_FilesPaths;

	std::vector<FSElement *> m_TreeElements;
	std::vector<File *> m_fileElements;
	std::vector<std::string> m_excludedFile;
	std::vector<std::string> m_excludedFolder;
	Status m_IsRecursive;
	std::string m_Filter;
};


class Files {
public:
	Files& operator-(const std::string& exclude);
	Files& operator+(const std::string& add);

	Files& operator-=(const std::string& exlude);
	Files& operator+=(const std::string& add);

	Status addFilter(FSElementFilter* filter);
	std::vector<File *>& getSourceFiles();
private:
	TreeFS m_Tree;
	std::vector<FSElementFilter *> m_filters;
};

#endif /* __COMMON_FILESYSTEM_H__ */
