#include <tfa_pr/PluginLoader.h>
#include <iostream>


int main (int argc, char **argv)
{
	PluginLoader myPlugin("tfa-plugins/plugin_test/libplugin_test.so");
	IPlugin *plugin;

	if (false == myPlugin.getPlugin(&plugin)) {
		return -1;
	}

	if (!plugin->initializePlugin()) {
		std::cout << "Error while initializing pluging" <<
			"tfa-plugins/plugin_test/libplugin_test" << std::endl;
		return -1;
	}

	TFAInfoPlugin info = plugin->getInfoPlugins();
	std::cout << "Plugin string ->" <<  info << std::endl;

	return 0;
}
