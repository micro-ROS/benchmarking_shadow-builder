#pragma once

#include <tfa/IPlugin.h> 
#include <tfa/ITFACommentListener.h>
#include <string>
#include <unordered_map>

class TFACMMyListener: public ITFACommentListener
{
public:
	TFACMMyListener():
		 ITFACommentListener("Benchmarking::User::HelloWorld") {}
	
	Status runnableComments(const TFACommentInfo& cleanComment,
				std::string& replacement);
private:
};

class MyPluginA: public IPlugin {
public:
	MyPluginA();
	~MyPluginA();

	TFAInfoPlugin& getInfoPlugins();
	bool initializePlugin();
};

extern "C" IPlugin* create() {
	return static_cast<IPlugin *>(new MyPluginA);
}
extern "C" void destroy(IPlugin* p) {
	delete p;
}
