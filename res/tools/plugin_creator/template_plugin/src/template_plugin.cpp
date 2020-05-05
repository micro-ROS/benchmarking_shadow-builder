/*
 * @brief This file is auto-generated to 
 */

#include <tfa/IPlugin.h>
#include <@@PlugName@@/@@PlugName@@.h>

#include <vector>
#include <iostream>
#include <new>

@@PlugName@@::@@PlugName@@() {}

@@PlugName@@::~@@PlugName@@()
{
	if(mInfos) {
		delete mInfos;
	}
}

TFAInfoPlugin& @@PlugName@@::getInfoPlugins()
{
//	return *mInfos;
}

bool MyPlugin::initializePlugin()
{
#if 0
	
	/** Inherited from the IPlugin */
	tbp = new TFABenchMarkingPlatform("*", "*", "*", "*");
	iclVect.emplace_back(static_cast<ITFACommentListener *>(new TFACMMonitorVar));
	iclVect.emplace_back(static_cast<ITFACommentListener *>(new TFACMMonitorTimer));

	mInfos = new TFAInfoPlugin("@@PlugName@@", *tbp);
#endif

	return Status::returnStatusOkay();
}

