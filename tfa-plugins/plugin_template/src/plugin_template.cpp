/*
 * @brief This file is auto-generated to 
 */

#include <tfa/IPlugin.h>
#include <inc/plugin_template.h>

#include <vector>
#include <iostream>
#include <new>

plugin_template::plugin_template() {}

plugin_template::~plugin_template()
{
	if(mInfos) {
		delete mInfos;
	}
}

TFAInfoPlugin& plugin_template::getInfoPlugins()
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

	mInfos = new TFAInfoPlugin("plugin_template", *tbp);
#endif

	return Status::returnStatusOkay();
}

