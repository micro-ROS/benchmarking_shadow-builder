/*
 * @brief This file is auto-generated to 
 */

#include <tfa/IPlugin.h>
#include <TimeBenchmarking/TimeBenchmarking.h>

#include <vector>
#include <iostream>
#include <new>

Timer::Timer()
 :
	 ITFACommentListener("Benchmarking::User::Timer")
{
}

Status Timer::runnableComments(const TFACommentInfo& cleanComment,
		std::string& replacement)
{
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
		printf(\"Exe time: %%ld sec : %%ld ms : %%ld ns\\n\",\n\
				result.tv_sec, result.tv_nsec / 1000000, result.tv_nsec\n\
				%% 1000000);\n\
	}\n";

		const std::vector<std::string> params = cleanComment.getParams();

	if (!params.size())
	{
		return Status::returnStatusError();
	}

	if (params.size() == 1) {
		if (params[0] == "declare" ) {
			replacement = "#include <time.h>\n";
			replacement += "#include <stdio.h>\n";
			return Status::returnStatusOkay();
		}
	} else if (params.size() == 2) {
		if (params[0]  == "start") {
			std::string start = "timer_start_" + params[1]; 
			std::string stop = "timer_stop_" + params[1]; 

			replacement = "struct timespec " + start + ", " + stop + ";\n";
			replacement += "\tclock_gettime(CLOCK_PROCESS_CPUTIME_ID, \
					&timer_start_" + params[1] + ");\n"; 
					return Status::returnStatusOkay();
		} else if (params[0]  == "stop") {
			std::string start = "timer_start_" + params[1]; 
			std::string stop = "timer_stop_" + params[1]; 
			char buf[sizeof(difftime_func) + start.length() +
				stop.length()];

			sprintf(buf, difftime_func, start.c_str(), stop.c_str());
			replacement += "clock_gettime(CLOCK_PROCESS_CPUTIME_ID, \
					&timer_stop_" + params[1] + ");\n";
			replacement += string(buf);

			return Status::returnStatusOkay();
		}
	}

	return Status::returnStatusError();
}


TimeBenchmarking::TimeBenchmarking() {}

TimeBenchmarking::~TimeBenchmarking()
{
	if(mInfos) {
		delete mInfos;
	}
}

TFAInfoPlugin& TimeBenchmarking::getInfoPlugins()
{
	return *mInfos;
}

bool TimeBenchmarking::initializePlugin()
{
	/* This is a plugin compatibility platform */
	tbp = new TFABenchMarkingPlatform("Linux", "*", "*", "*");

	/** Here register the Timer listener */
	iclVect.emplace_back(static_cast<ITFACommentListener *>(new
					Timer));

	/* This is the infoPlugin that holds the plugin name and the
		platform information */
	mInfos = new TFAInfoPlugin("Test Plugin", *tbp);

	// Will be explained later how to mock up this part.
	return Status::returnStatusOkay();
}

