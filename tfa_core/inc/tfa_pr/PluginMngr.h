#pragma once
#include <common/Status.h>
#include <common/FileSystem.h>

#include <tfa/IPlugin.h>
#include <tfa/TFABenchMarkingPlatform.h>
#include <tfa_pr/PluginLoader.h>

#include <unordered_map>

class PluginMngr {
public:
	static Status pluginMngrFindPlugins(const string& pathToSearch)
	{
		if (mPlugInstance == nullptr)
		{
			std::cout << "PluginMngr not initalised"
				<< std::endl;
			return Status::returnStatusError();
		}

		TreeFS tfs;
		std::vector<FSElementFilter *> filters = {
			new FSElementFilterExt(".so", false)
		};

		if (tfs.addPath(pathToSearch, true)) {
			std::cout << "Could not find anything"
				<< std::endl;
			return Status::returnStatusError();
		}

		tfs.searchFiles(filters);
		for (const File* file: tfs.getListOfFiles()) {
			std::cout << "Found plugin" << std::endl;
			PluginLoader *pl = new PluginLoader(file->getCurrentPath());
			IPlugin* ip ;

			if (!pl->getPlugin(&ip)) {
				std::cout << "No path found" << file->getCurrentPath() << std::endl;
				continue;
			}

			std::cout << "Found plugin" << std::endl;
			if (ip->initializePlugin()) {
				std::cout <<  " "
				<< "Error while initialising plugin"
				<< file->getCurrentPath()
				<< std::endl;

				continue;
			}

			mPlugInstance->mPlugList.emplace(ip, pl);
		}

		return Status::returnStatusOkay();
	}

	static const Status removePlugin(IPlugin *plug)
	{
		if (mPlugInstance == nullptr)
		{
			std::cout << "PluginMngr not initalised"
				<< std::endl;
			return Status::returnStatusError();
		}

		if (mPlugInstance->mPlugList.find(plug) 
				== mPlugInstance->mPlugList.end()) {
			std::cout << "Not such a plugin" << std::endl;
			return Status::returnStatusError();
		}

		PluginLoader *pl = mPlugInstance->mPlugList[plug];
		mPlugInstance->mPlugList.erase(plug);
		delete pl;

		return Status::returnStatusOkay();
	}

	static Status getListOfPluginMatching(std::vector<IPlugin*>& pl,
						const TFABenchMarkingPlatform& plat)
	{
		if (mPlugInstance == nullptr)
		{
			std::cout << "PluginMngr not initalised" << std::endl;
			return Status::returnStatusError();
		}

		for (auto it : mPlugInstance->mPlugList) {
			if (plat == it.first->getInfoPlugins().getTFABMPlat()) {
				pl.emplace_back(it.first);
			}
		}

		return Status::returnStatusOkay();
	}

	static Status initPluginMngr()
	{
		if (mPlugInstance != nullptr)
			return Status::returnStatusError();

		mPlugInstance = new PluginMngr();

		return Status::returnStatusOkay();
	}


private:
	PluginMngr() {}

	static PluginMngr *mPlugInstance;
	std::unordered_map<IPlugin *, PluginLoader *> mPlugList;
};
