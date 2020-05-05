#pragma once

#include <tfa/IPlugin.h> 
#include <tfa/ITFACommentListener.h>
#include <string>

class Timer: public ITFACommentListener
{
public:
	Timer();
	Status runnableComments(const TFACommentInfo& cleanComment,
				std::string& replacement);
};

class TimeBenchmarking: public IPlugin {
public:
	TimeBenchmarking();
	~TimeBenchmarking();

	TFAInfoPlugin& getInfoPlugins();
	bool initializePlugin();
};

extern "C" IPlugin* create() {
	return static_cast<IPlugin *>(new TimeBenchmarking);
}
extern "C" void destroy(IPlugin* p) {
	delete p;
}
