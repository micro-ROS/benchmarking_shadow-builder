#pragma once

#include <tfa/IPlugin.h> 

class @@PlugName@@: public IPlugin {
public:
	@@PlugName@@();
	~@@PlugName@@();

	TFAInfoPlugin& getInfoPlugins();
	bool initializePlugin();
private:
};

extern "C" IPlugin* create() {
	return static_cast<IPlugin *>(new @@PlugName@@);
}
extern "C" void destroy(IPlugin* p) {
	delete p;
}
