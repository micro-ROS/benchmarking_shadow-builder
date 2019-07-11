#pragma once

#include <tfa/IPlugin.h> 
#include <tfa/ITFACommentListener.h>
#include <string>

class plugin_template: public IPlugin {
public:
	plugin_template();
	~plugin_template();

	TFAInfoPlugin& getInfoPlugins();
	bool initializePlugin();
private:
};

extern "C" IPlugin* create() {
	return static_cast<IPlugin *>(new @PlugName@@);
}
extern "C" void destroy(IPlugin* p) {
	delete p;
}
