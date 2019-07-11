#ifndef __SHADOW_BUILDER_CODEPARSER_H__
#define __SHADOW_BUILDER_CODEPARSER_H__

#include <common/Status.h>
#include <vector>
#include <common/FileSystem.h>

#if 0
class SourceDir {
public:
	Status addFile(std::string& pathFile, bool isRecursive = false);
	Status addFolder(std::string& pathFolder, bool isRecursive = false);

	/** Remove a file or a folder form the list */
	Status excludeFolder(std::string& pathFolder, bool isRecursive = false);
	Status excludeFile(std::string& pathFile, bool isRecursive = false);

	std::vector<std::string>& getListSourceFile() { return m_vecSourcesPath; }
private:
	Status retrieveListSourceFile(std::string& path, bool isRecursive);

	std::vector<std::string> m_vecExcludedPath;
	std::vector<std::string> m_vecSourcesPath;
	std::vector<std::string> m_vecFolderPath;
};

/** This is the project to which are bound sources */
class ProjectSrc: public SourceDir {
	ProjectSrc();
};
#endif

class ICodeParser {
public:
	ICodeParser() {}
	virtual Status initAndStartCodeparser() = 0;
	void registerSourcePaths(std::vector<File *>& files) { m_sourcesPaths = &files; }

protected:
	std::vector<File *> *m_sourcesPaths;
};

/** Maybe use flex/bison? */
class ClangParser: public ICodeParser
{
public:
	virtual Status initAndStartCodeparser();
private:
};

class Parser
{
public:
	typedef enum {
		PARSER_CLANG,
		PARSER_FLEXBISON,
	} TypeParser;

	static Status setParser()
	{
		return Status::returnStatusOkay();
	}

	static Status setFilesToAnalyse(std::vector<File *>& files)
	{
		ICodeParser& parser = *static_cast<ICodeParser *>(&mparser);
		parser.registerSourcePaths(files);
		return Status::returnStatusOkay();
	}

	static Status startParsing()
	{
		ICodeParser& parser = *static_cast<ICodeParser *>(&mparser);
		return parser.initAndStartCodeparser();

	}
private:
	static ClangParser mparser;
};


#endif /* __SHADOW_BUILDER_CODEPARSER_H__*/
