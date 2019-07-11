#pragma once

#include <tfa/IPlugin.h>
#include <string>

class PluginLoader {
public:
	PluginLoader(const std::string& name)
		:
			mLoadedPlugin(NULL),
			mPluginName(name), mLibraryHandle(NULL),
			mCreatePlugin(NULL), mDestroyPlugin(NULL)
	{
	}

	~PluginLoader();

	bool getPlugin(IPlugin **container);

private:

	IPlugin *mLoadedPlugin;
	std::string mPluginName;
	void *mLibraryHandle;
	create_t *mCreatePlugin;
	destroy_t *mDestroyPlugin;
};
