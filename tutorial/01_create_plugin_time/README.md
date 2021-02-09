## Creation of a plugin

This section deals with one specific benchmarking tool called the
Shadow Builder. More specifically, this tutorial aims to create a plugin from
A to Z and explain how to instrument your code.

For the sake of ease of understanding, this tutorial is proposing to
benchmark the time spent on a simple looping function.

## Prerequisite

Before getting to the heart of the matter, it is needed to meet the following
requirements:

 1. It is assumed that the **shadow builder** and **trace framework abstraction**
terminology and mechanisms are known. If this is not the case, the
documentation related to the benchmarking is available
[here](https://github.com/micro-ROS/benchmarking_shadow-builder/blob/master/README.md).

 2. Using linux, preferably Linux Ubuntu 18.04 and above, all debian based
    distros should do the job.

 3. Some knowledge about c and c++ programing


Once all the checkboxes are ticked, the tutorial can begin.

## TFA - Plugin

In order to create a plugin, the information that is crucial to figure out is as follows:

What is to be benchmarked? --> The time spent in a function.
How to do it? --> Is there a plugin already supporting it? If yes, then go ahead and
the code to the profile can be instrumented.

If no plugin supports it, then a plugin has to be created.
Then, another set of questions arises, which depend on the context:

 1. How could it benifit the others? 
 2. What piece of code would be used to measure the time? (in C or C++).
 3. What platform it can support? (OS, CPU, etc...).
 4. How the code shall be instrumented?


The answers to these questions would be:

 1. Create a generic plugin and write a documentation, that would be
    understandable both to a normal and an expert user.
 2. Use the _timespec_ and _clock_gettime_ Linux syscall.
 3. From previous answer --> OS: Linux and CPU as long as it has the same Linux api.
 4. Use a simple way applying the comment as follows: 
    _/** Benchmarking::plugin_name::function */_ . `The choice for the current
    tutorial would be:` _/** Benchmarking::TimeBenchmarking::Timer */_

 

These answers provide us with the minimum necessary details for the plugin creation.

# Create a tfa - plugin

## Files tree structure

The final code shall be located in the folder path
`src_root_sb/tfa-plugin/TimeBenchmarking` with the following structure:

TimeBenchmarking \
	├── CMakeLists.txt \
	├── inc \
	│   └──TimeBenchmarking \
	│       └── TimeBenchmarking.h \
	└── src \
	    └── plugin_TimeBenchmarking.cpp \

## Register a new plugin into the TFA core of the shadow builder

The shadow-builder is relying on TFA's plugins execution to answer the
parser dispatchment. Therefore, the need for some interoperability exists.

Every new plugin is written by implementing the IPlugin interface as shown in
the file `src_root/tfa_core/inc/tfa/IPlugin.h`. All the interface needs to do, is to
implement the pure virtual functions. A simple example would be as in the
_plugin_test_.

In the plugin header:

``` cpp
class TimeBenchmarking: public IPlugin {
public:
	TimeBenchmarking();
	~TimeBenchmarking();

	TFAInfoPlugin& getInfoPlugins();
	bool initializePlugin();
private:
};

extern "C" IPlugin* create() {
	return static_cast<IPlugin *>(new TimeBenchmarking);
}
extern "C" void destroy(IPlugin* p) {
	delete p;
}
``` 
In the plugin source code:

``` cpp
TimeBenchmarking::TimeBenchmarking() {}

TimeBenchmarking::~TimeBenchmarking()
{
	if(mInfos) {
		delete mInfos;
	}
}

TFAInfoPlugin& TimeBenchmarking::getInfoPlugins()
{
	return *mInfo;
}

bool TimeBenchmarking::initializePlugin()
{	
	/* This is a plugin compatibility platform */
	tbp = new TFABenchMarkingPlatform("Linux", "*", "*", "*")

	/* This is the infoPlugin that holds the plugin name and the
		platform information */
`	mInfos = new TFAInfoPlugin("Test Plugin", *tbp);

	// Will be explained later how to mock up this part.
	return Status::returnStatusOkay();
}
```

## Create a listener

Good! Now the plugin is ready to be registered within the TFA's core. So when a
session is running, the plugin will be found. However, nothing will really
happen. Indeed, your plugin is not listening to a specific tag yet.

Just as a reminder, the listener is an object derivated from the interface
**ITFACommentListener**.  It is listening to a specific _Tag_, which will be
replaced by a piece of code.

The declaration of the object shall be as below:
``` cpp
class Timer: public ITFACommentListener
{
public:
	Timer();
	Status runnableComments(const TFACommentInfo& cleanComment,
				std::string& replacement);
private:
};
```

As shown above, the class **Timer** inherits from the **ITFACommentListenner** class, 
which has one pure virtual method called _runnableComments_.
This means your plugin has to implement the method _runnableComments(...)_.


``` cpp

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
		       	result.tv_sec, result.tv_nsec / 1000000, result.tv_nsec \
%% 1000000);\n\
}\n" ;

Timer::Timer()
 :
	 ITFACommentListener("Benchmarking::User::Timer")
{
}

Status Timer::runnableComments(const TFACommentInfo& cleanComment,
		std::string& replacement)
{
	return Status::returnStatusError();
}
```

Now the functions are correctly implemented. The timer needs several things to
measure the time spent in a function:

 1. Start the timer before the function, get an intial timestamp..
 2. Stop the timer after the function has returned, get another timestamp
 3. Calculate the delta between the two timestamps measured above.
 4. Print the delta in a human readable way.

This basically means that the plugin will neeed a way to get the timestamps, as
discussed before, by using the _clock_gettime_, and print it to the user by using
_printf_.

A tag can provide several parameters. This will be usefull for the sack of
the timer:

 * A parameter to identify what the timer's status is (i.e. start or stop).
 * A parameter to identify the timer itself in a unique way by the dev.
 * A parameter that is needed for header declaration.

Without any benchmark tools, this code would look like as follows: 

```c

#include <time.h> // Needed to access the clock_gettime() function
#include <stdio.h> // Needed to access the printf function.

void func2benchmark(...)
{

	/** declare and measure the starting timestamp */
	struct timestamp timer_start, timer_stop;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer_start);
	// do something very slow

	/** Measure the timesampe now, process and show the results */
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer_start);
	{
		struct timespec *start = &timer_start;
		struct timespec *stop = &timer_stop;
		struct timespec result;

		if ((stop->tv_nsec - start->tv_nsec) < 0) {
			result.tv_sec = stop->tv_sec - start->tv_sec - 1;
			result.tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
		} else {
			result.tv_sec = stop->tv_sec - start->tv_sec;
			result.tv_nsec = stop->tv_nsec - start->tv_nsec;
		}

		printf(\"Exe time: %ld sec : %ld ms : %ld ns\\n\",
				result.tv_sec, result.tv_nsec / 1000000, result.tv_nsec % 1000000);
	}
}
```

Using the TFA, the code would look like this:

```c
/** Benchmarking::TimeBenchmarking::Timer(declare) */

void func2benchmark(...)
{
/** Benchmarking::TimeBenchmarking::Timer(start, timer1) */
	//do something very slow
/** Benchmarking::TimeBenchmarking::Timer(stop, timer1) */
}
```

By taking a look at the differences between both codes, the amount of overhead code
introduced by the benchmark method is very low. 

Now, let's roll-up the sleeves and implement it as a plugin and therefore in a
generic way to be reusable.

### Declare

Header files need to be declared in the benchmark code. In order to do this, the parameter 0 must equal "declare". The replacement
will be `"#include \<time.h\>\n#include \<stdio.h\>\n"` 

It is needed to append "\n" to the end of a line, as this piece of code is going
to be appended to the another code. 

```cpp

Status Timer::runnableComments(const TFACommentInfo& cleanComment,
		std::string& replacement)
{
	const std::vector<std::string> params = comment.getParams();

	if (params[0] == "declare" && params.size() == 1) {
		replacement = "#include <time.h>\n";
		replacement += "#include <stdio.h>\n";
		return Status::returnStatusOkay();
	}

	return Status::returnStatusError();
}
```

It is mandatory to return _Status::returnStatusOkay()_ to tell the _tfa-core_ that
the _Tag_ was handled and therefore no other plugins will be using it.

#### Start 

The starting element will basically record a timestamp in the memory. How to do
so in C programming on a Linux system would be as follows:

```cpp
Status Timer::runnableComments(const TFACommentInfo& cleanComment,
		std::string& replacement)
{
	const std::vector<std::string> params = comment.getParams();


	if (params[0]  == "start" && params.size() == 2) {
		std::string start = "timer_start_" + params[1]; 
		std::string stop = "timer_stop_" + params[1]; 

		replacement = "struct timespec " + start + ", " + stop + ";\n";
		replacement += "\tclock_gettime(CLOCK_PROCESS_CPUTIME_ID,
			    &timer_start_" + params[1] + ");\n";
		return Status::returnStatusOkay();
	}

	return Status::returnStatusError();
}
```

It is mandatory to return _Status::returnStatusOkay()_ to tell the _tfa-core_ that
the _Tag_ was handled and therefore no other plugins will be using it.

#### Stop

Then the stopping element, which will be in charge of getting a timestamp,
calculates the delta time spent between the stop and the start and finally prints it in a
human readable way.

```cpp

Status Timer::runnableComments(const TFACommentInfo& cleanComment,
		std::string& replacement)
{
	const std::vector<std::string> params = comment.getParams();
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
				result.tv_sec, result.tv_nsec / 1000000, result.tv_nsec
				%% 1000000);\n\
	}\n";

	if (params[0]  == "stop" && params.size() == 2) {
		std::string start = "timer_start_" + params[1]; 
		std::string stop = "timer_stop_" + params[1]; 
		char buf[sizeof(difftime_func) + start.length() +
			stop.length()];

		sprintf(buf, difftime_func, start.c_str(), stop.c_str());
		replacement += "clock_gettime(CLOCK_PROCESS_CPUTIME_ID,
			    &timer_stop_" + params[1] + ");\n";
		replacement += string(buf);

		return Status::returnStatusOkay();
	}

	return Status::returnStatusError();
}
```

It is mandatory to return _Status::returnStatusOkay()_ to tell the _tfa-core_ that
the _Tag_ was handled and therefore no other plugins will be using it.

Additionally, it is necessary to consider that the replacement code, which is going to be compiled, is actually
C code. Therefore, one should be careful about the way of formatting it and using the escape characters.

#### Combine everything together

Finally the whole _runnableComment_ method will be look like that:

```cpp
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
				result.tv_sec, result.tv_nsec / 1000000, result.tv_nsec
				%% 1000000);\n\
	}\n";

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
		replacement += "\tclock_gettime(CLOCK_PROCESS_CPUTIME_ID,
				&timer_start_" + params[1] + ");\n";
		return Status::returnStatusOkay();
	} else if (params[0]  == "stop" && params.size() == 2) {
		std::string start = "timer_start_" + params[1]; 
		std::string stop = "timer_stop_" + params[1]; 
		char buf[sizeof(difftime_func) + start.length() +
			stop.length()];

		sprintf(buf, difftime_func, start.c_str(), stop.c_str());
		replacement += "clock_gettime(CLOCK_PROCESS_CPUTIME_ID,
					&timer_stop_" + params[1] + ");\n";
		replacement += string(buf);
		
		return Status::returnStatusOkay();
	}

	return Status::returnStatusError();
}
```

## Register the listener

Once the listener is implemented, then it needs to be registerd within the
plugin:

```cpp
bool TimeBenchmarking::initializePlugin()
{	
	/* This is a plugin compatibility platform */
	tbp = new TFABenchMarkingPlatform("Linux", "*", "*", "*")

	/** Here register the Timer listener */
	iclVect.emplace_back(static_cast<ITFACommentListener *>(new
					Timer));

	/* This is the infoPlugin that holds the plugin name and the
		platform information */
`	mInfos = new TFAInfoPlugin("Test Plugin", *tbp);

	// Will be explained later how to mock up this part.
	return Status::returnStatusOkay();
}
```

The protected vector _iclVect_ from the **IPlugin** class needs to be appended with each listener, which will be
supported and implemented in the plugin.


## Compilation files

The compilation file will be the _CMakeLists.txt_ located in the root  folder of the plugin.
It shall look like the following:

```cmake
cmake_minimum_required(VERSION 3.13)  # CMake version check
project(TimeBenchmarking VERSION 0.1 DESCRIPTION "MY Plugin")
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

set(PLUGIN_NAME "TimeBenchmarking")

# Needed to get the function to do tests 
include(../../CMakeMacros/CMakeTesting.txt)

# Plugins include folders
include_directories(inc/)

# Plugins source files
list(APPEND TEST_PLUGIN_SRC
        src/TimeBenchmarking.cpp
)

# Needed to create a shareable library
add_library(${PLUGIN_NAME} SHARED ${TEST_PLUGIN_SRC})

# Target library that we need to link against
target_link_libraries(tfa)

# Needed to create a version of the shared library
set_target_properties(${PLUGIN_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})
```

And finally, it is needed to add into the parent folder's _CMakeLists.txt_ the
subdirectory of the plugin:

```cmake
cmake_minimum_required(VERSION 3.10)  # CMake version check
set(CMAKE_CXX_STANDARD 14) # Enable c++14 standard

add_subdirectory(plugin_test)
add_subdirectory(myplugin)

# Add your project configuration here:
add_subdirectory(TimeBenchmarking)
```

### Compilation
To compile the plugin, run the following commands from the build folder created before in the _shadow-builder_ root folder:

```shell
cd src_root/build/ 
cmake ..
make -j4
```
## Configuration

### TFA configuration

An example of the configuration file is in the source tree at `src_root/res/tfa-res/tfa.xml`.

This file only keeps a track of the path where to look for plugins. Watch out! This
file is a template and it is renewed at each compilation. 

In the current context, the path is the default one.


### The shadow builder configuration

The shadow-builder configuration provides some hints where the source files
to benchmark can be found and where the ouput folder should be set.

A detailed explaination can be found
[here](https://github.com/micro-ROS/benchmarking_shadow-builder/blob/master/res/README.md#shadow-builder-configuration).

## Running the shadow-builder
Once all the steps above are done and the plugin is compiled, the command to run the code’s instrumentation would be:

```shell
cd src_root/build/ 
./shadow-program -s ../res/sb-res/bcf.xml -t ../res/tfa-res/tfa.xml
```

The output, configured in the **bcf.xml** configuration file, should be by default put into the folder **/tmp/output/** under the session's name **test**  appended by the date and time when the benchmarking was started.

