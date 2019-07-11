#pragma once

#include <tfa/IPlugin.h> 
#include <tfa/ITFACommentListener.h>
#include <string>
#include <unordered_map>

class TFACMMonitorTimer: public ITFACommentListener
{
public:
	TFACMMonitorTimer():
		 ITFACommentListener("Benchmarking::User::Timer") {}
	
	Status runnableComments(const TFACommentInfo& cleanComment,
				std::string& replacement);
private:
};

class TFACMMonitorVar: public ITFACommentListener
{
public:
	TFACMMonitorVar():
		 ITFACommentListener("Benchmarking::User::Monitor") {}

	Status runnableComments(const TFACommentInfo& comment,
				std::string& replacement);

};

class MyPlugin: public IPlugin {
public:
	MyPlugin();
	~MyPlugin();

	TFAInfoPlugin& getInfoPlugins();
	bool initializePlugin();
private:
};

extern "C" IPlugin* create() {
	return static_cast<IPlugin *>(new MyPlugin);
}
extern "C" void destroy(IPlugin* p) {
	delete p;
}
