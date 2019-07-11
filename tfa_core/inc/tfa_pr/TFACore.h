#ifndef __TFA_H__
#define __TFA_H__

#include <common/Status.h>
#include <tfa_pr/PluginMngr.h>
#include <tfa/ITFACommentListener.h>
#include <string>

#include <vector>
class TFAConfiguration {
public:
	TFAConfiguration(const std::string pluginsFolder, 
			TFABenchMarkingPlatform& TFABMPlat)
		:
		mPluginsFolder(pluginsFolder),
		mPlat(TFABMPlat)
	{;}

	TFAConfiguration& operator=(const TFAConfiguration& A)
	{
		mPluginsFolder = A.mPluginsFolder;
		mPlat = A.mPlat;
		return *this;
	}
	const std::string& getPluginsFolder() { return mPluginsFolder; }
	const TFABenchMarkingPlatform& getBMPlat() { return mPlat; }
private:
	std::string mPluginsFolder;
	TFABenchMarkingPlatform mPlat;
};

class TFACore {
public:
	static Status initTFACore(TFAConfiguration& TFAConfig)
	{
		if (tfaCoreInstance != nullptr)
			return Status::returnStatusError();

		tfaCoreInstance = new TFACore();
		PluginMngr::initPluginMngr();
		PluginMngr::pluginMngrFindPlugins(TFAConfig.getPluginsFolder());

		PluginMngr::getListOfPluginMatching(tfaCoreInstance->mPluginsList,
					TFAConfig.getBMPlat());

		return Status::returnStatusOkay();
	}

	static Status getReplacement(std::string& comment,
					   std::string& replacement)
	{
		if (nullptr == tfaCoreInstance) {
			std::cout << "TFACore not allocated, Was the TFACore initialised?" << std::endl;
			return Status::returnStatusError();
		}

		common::StringMan::trimSpace(comment);
		common::StringMan::removeCommentBeacons(comment);
		std::cout << "[TFACore]: Received comment \"" << comment << "\"" << std::endl;
		if (comment.find("Benchmarking::") == std::string::npos) {
			std::cout << "Not a shadow-builder instruction" << std::endl;
			return Status::returnStatusError();
		}

		TFACommentInfo cleanComment(comment);

		for(IPlugin* plugin: tfaCoreInstance->mPluginsList) {
			if (plugin->dispatchComment(cleanComment, replacement)) {
				std::cout << "Found comment" << std::endl;
				break;
			}
		}

		return Status::returnStatusOkay();
	}

private:
	TFACore() {;}
	static TFACore *tfaCoreInstance;
	std::vector<IPlugin*> mPluginsList;
};


#endif /* __TFA_H__ */
