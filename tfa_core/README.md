Trace Framework Abstraction
===========================

The Trace Framework Abstraction (TFA) is layer that will allow any tracing
tool to be used during benchmarking. The TFA provide the structure and the
frame into which a developper might have to implement plugins.
that are loaded upon start up.

# TFA implementation

The TFA is defined in a configuration file and the implementation of the
translation functions are written in a library that acts as a plugging.

## TFA core

The core will be in charge of loading the different plugins and calling the
callbacks from the plugins to generate a new source code that would comply with
the needs of the benchmarking. In addition, the core will generate a script that
describes the action to take with the binary to generate the needed benchmarking
information.

## TFA Tree view

tfa_core \
	├── inc \
	│   ├── tfa 	--> Public interface that can be accessed by a Plugin \
	│   └── tfa_pr  --> Private interface, only used by the shadow-builder \
	└── src 	--> Source files of the tfa module

## TFA Public interface and Frameworks rules

In order to implement a plugin it mandatory to implement two interface classes:

**IPlugin**: Two functions need to be implemented :

 * _virtual bool initializePlugin()_ This is the function that
   initiliazes the plugin.
 
 * _virtual TFAInfoPlugin& getInfoPlugins()_ providing a list TFAInfoPlugin
   that is telling what kind of plugin is it, what platform it supports
   etc....

**ITFACommentListener**: This is a comment listener interface. The comment to listen to
is a tag. The tag needs to follow the pattern as described in the main
[../README.md#Tags](../README.md#tags). From this class, the plugin must
implement the function:

 * _virtual Status runnableComments(const TFACommentInfo& comment, std::string& replacement)_:
   This function will be called during dispatching matching the string that
   was provided during the creation of the ITFACommentListener.

 * In the constructor of the derived class, the derived class must call the
   constructor of ITFACommentListener with the string that the plugin wants to
   respond to.

Each ITFACommentListener implemented class need to register it in its plugin
using adding it in the iclVect that is a protected member of the IPlugin class.

Additionnaly, the  plugin must allocate a new TFABenchMarkingPlatform(...) to
get informaton additional information regarding the platform that is supported.

The TFABenchMarkingPlatform is an object that needs 4 element during its
creating: 

 * std::string& OSName: The name of the of the os (i.e. "Linux, Nuttx, et.c ...)
 * std::string& CPUArch: Architecture of the platform: (i.e. "X86_64, ARM, et.c ...) 
 * std::string& CPUName: CPUNAme Cpu model (i.e "i7700K, arm-cortex-m4", et.c ...),
 * std::string& boardName: The board on which its running if there is (i.e. "olimex, lauchpad, SOM et.c ...).

Note that if any of these parameters are filled with a start "\*", that would
mean **do not care**. This way this parameter is not checked and the plugin can
be processed.

## TFA Behavior

### During initialisation

During the initiliasation of the shadow-buidler, the TFA core will look for all
plugins. A plugin that has inherited from the IPlugin.h as described in the
section [TFA Public interface and Frameworks rules](#tfa_public_interface_and_frameworks_rules)
will be called by the core and the implemented function initializePlugin()  will
be called. Aftewards, the function getInfoPlugin will be check to get the name
of the plugin.

**Important Note**: The TFA core is expecting that the plugin register the list
of _listeners_ during the initializePlugin in its iclVect vector, as well as
allocating the *tbp* (TFABenchMarkingPlatform\*) which is inherited. An
example:

``` cpp
/* Here just allocating the platform */
	tbp = new TFABenchMarkingPlatform("Nuttx", "arm-m4", "*", "*");

/* Here adding listener element to the vector */
	ICLVect.emplace_back(static_cast<ITFACommentListener *>(new TFACMMonitorVar));
```

### While parsing code

This section is describing the way how the TFA core is behaving at runtime. 

The TFA core receives a comment (not yet known if a **Tag** or not).
If the comment starts by **/\*\* Bechmarking::** the TFA core considers it a **Tag**.

The tag is proceessed (meaning that it's space trimmed, param extract if any and
removing the comment delimiters).

Once processed, the TFA core is looping through all the possible plugins that are availabe.
Then the TFA core will look at all registered element in it all plugin's iclVect.

If any plugin has registered match in the iclVect, then it's passed to the plugin.

Then the plugin processes the **Tag** received. Each time a plugin in not interested by the **Tag**
it should respond returning the *Status::returnStatusError()*. In this way the TFA core 
is continuing to loop through the remaining plugins. 

If a Plugin is interested by **Tag**, it will fill the replacement and then it
shall return *Status::returnStatusOkay()*.  This way the TFA core does not
continue looping and return to the file rewriter and parser to continue to the next
_Tag_ found.
