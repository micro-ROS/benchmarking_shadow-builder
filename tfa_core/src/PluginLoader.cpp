#include <tfa_pr/PluginLoader.h>

#include <dlfcn.h>
#include <iostream>
#include <malloc.h>

PluginLoader::~PluginLoader()
{
	if (mLoadedPlugin) {
		mDestroyPlugin(mLoadedPlugin);
		mLoadedPlugin = NULL;
	}

	if (mLibraryHandle) {
		dlclose(mLibraryHandle);
		mLibraryHandle = NULL;
	}
}

bool PluginLoader::getPlugin(IPlugin **container)
{	
	char* dlsym_error;
	bool rc = true;

	std::string path = mPluginName;
	mLibraryHandle = dlopen(path.c_str(), RTLD_LAZY);

	if (!mLibraryHandle) {
		std::cerr << "Cannot open library: " << dlerror() << std::endl;
		rc = false;
		goto failed_finding_library;
	}

	std::cout << "Loading symbols" << std::endl;

	mCreatePlugin = (create_t*) dlsym(mLibraryHandle, "create");
	dlsym_error = dlerror();

	if (dlsym_error || !mCreatePlugin) { 
		std::cerr << "Erorr " << dlsym_error << std::endl;
		mCreatePlugin = NULL;
		rc = false;
		goto failed_finding_symbols;	
	}

	mDestroyPlugin = (destroy_t*) dlsym(mLibraryHandle, "destroy");
	dlsym_error = dlerror();

	if (dlsym_error || !mDestroyPlugin) { 
		std::cerr << "Erorr " << dlsym_error << std::endl;
		mDestroyPlugin = NULL;
		rc = false;
		goto failed_finding_symbols;	
	}

	 mLoadedPlugin = mCreatePlugin();
	 if (!mLoadedPlugin) {
		rc = false;
		goto failed_create_plugin;
	 }

	std::cout << "Pluging loaded and created" << std::endl;
	*container = mLoadedPlugin;

	return rc;

failed_finding_symbols:
failed_create_plugin:
	free(mLibraryHandle);
	mLibraryHandle = NULL;
failed_finding_library:
	return rc;

}
