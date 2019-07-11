#include <tfa/IPlugin.h>
#include <plugin_test.h>

#include <vector>
#include <iostream>
#include <new>

const char difftime_func[] =
"{\n\
	struct timespec *start = &%s;\n\
	struct timespec *stop = &%s;\n\
	struct timespec result;\n\
	if ((stop->tv_nsec - start->tv_nsec) < 0) {\n\
		result.tv_sec = stop->tv_sec - start->tv_sec - 1;\n\
		result.tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;\n\
	} else {\n\
		result.tv_sec = stop->tv_sec - start->tv_sec;\n\
		result.tv_nsec = stop->tv_nsec - start->tv_nsec;\n\
	}\n\
	#include <stdio.h>\n\
	printf(\"Exe time: %%ld sec : %%ld ms : %%ld ns\\n\",\n\
		       	result.tv_sec, result.tv_nsec / 1000000, result.tv_nsec %% 1000000);\n\
}\n" ;

MyPlugin::MyPlugin() {}

MyPlugin::~MyPlugin()
{
	if(mInfos) {
		delete mInfos;
	}
}

TFAInfoPlugin& MyPlugin::getInfoPlugins()
{
	return *mInfos;
}

bool MyPlugin::initializePlugin()
{
	/** Inherited from the IPlugin */
	tbp = new TFABenchMarkingPlatform("Linux", "x86_64", "Intel", "Std Processor");
	iclVect.emplace_back(static_cast<ITFACommentListener *>(new TFACMMonitorVar));
	iclVect.emplace_back(static_cast<ITFACommentListener *>(new TFACMMonitorTimer));

	mInfos = new TFAInfoPlugin("Test Plugin", *tbp);

	return Status::returnStatusOkay();
}

Status TFACMMonitorTimer::runnableComments(const TFACommentInfo& comment,
		std::string& replacement)
{
	std::cout << "Var Monitor timer" << std::endl;
	std::cout << "Received comment:" << comment.getCommentRaw() << std::endl;
	std::cout << *this << std::endl;

	const std::vector<std::string> params = comment.getParams();

	if (!params.size())
	{
		return Status::returnStatusError();
	}

	if (params[0] == "declare" && params.size() == 1) {
		replacement = "#include <time.h>\n";
		replacement += "#include <stdio.h>\n";
		return Status::returnStatusOkay();
	} else if (params[0]  == "start" && params.size() == 2) {
		std::string start = "timer_start_" + params[1]; 
		std::string stop = "timer_stop_" + params[1]; 

		replacement = "struct timespec " + start + ", " + stop + ";\n";
		replacement += "\tclock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer_start_" + params[1] + ");\n";
		return Status::returnStatusOkay();
	} else if (params[0]  == "stop" && params.size() == 2) {
		std::string start = "timer_start_" + params[1]; 
		std::string stop = "timer_stop_" + params[1]; 
		char buf[sizeof(difftime_func) + start.length() + stop.length()];

		sprintf(buf, difftime_func, start.c_str(), stop.c_str());
		replacement += "clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer_stop_" + params[1] + ");\n";
		replacement += string(buf);
		
		return Status::returnStatusOkay();
	}

	return Status::returnStatusError();
}

Status TFACMMonitorVar::runnableComments(const TFACommentInfo& comment,
		std::string& replacement)
{
	const std::vector<std::string> params = comment.getParams();

	std::cout << "Var Monitor Variable" << std::endl;
	std::cout << "Received comment:" << comment.getCommentRaw() << std::endl;
	std::cout << *this << std::endl;

	if (params.size() == 1) {
		if (params[0] == "declare") {
			replacement = "#include <stdio.h>\n";
			return Status::returnStatusOkay();
		}		
	} else if (params.size() == 2) {
		if (params[0] == "U32") {
			replacement = "printf(\"Monitor var " + params[1] + ": %u\\n\", " + params[1] + ");\n";
			return Status::returnStatusOkay();
		} else {
			replacement = "";
		}

	}

	return Status::returnStatusError();
}

