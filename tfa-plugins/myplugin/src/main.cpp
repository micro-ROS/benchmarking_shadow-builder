#include <plug-inc/plug.h>
#include <tfa/IPlugin.h>

#include <vector>
#include <iostream>
#include <new>

Status TFACMMyListener::runnableComments(const TFACommentInfo& comment,
		std::string& replacement)
{

	const std::vector<std::string> params = comment.getParams();

	if (params.size() == 1) {
		if (params[0] == "declare") {
			replacement = "#include <stdio.h>\n";

			return Status::returnStatusOkay();
		} else if (params[0] == "show") {
			replacement = "printf(\"Hello World\");\n";

			return Status::returnStatusOkay();
		}
	}

	return Status::returnStatusError();
}



TFAInfoPlugin& MyPluginA::getInfoPlugins()
{
	return *mInfos;
}

bool MyPluginA::initializePlugin()
{
	/** Inherited from the IPlugin */
	tbp = new TFABenchMarkingPlatform("Linux", "X86_64", "*", "*");

	/** Inherited from the IPlugin */
	iclVect.emplace_back(static_cast<ITFACommentListener *>(new TFACMMyListener));

	mInfos = new TFAInfoPlugin("MyPlugin 1", *tbp);

	return Status::returnStatusOkay();
}

MyPluginA::MyPluginA() {}

MyPluginA::~MyPluginA()
{
	if(mInfos) {
		delete mInfos;
	}
}
