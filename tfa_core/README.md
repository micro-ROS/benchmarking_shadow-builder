Trace Framework Abstraction
===========================

The Trace Framework Abstraction (TFA) is a layer that will allow any tracing
tool to be used during benchmarking. The TFA provides the structure and the
frame into which a developer might have to implement plugins, which are loaded upon start up.

# TFA implementation

The TFA is defined in a configuration file and the implementation of the
translation functions are written in a library that acts as plugins.

## TFA core

The TFA core will be in charge of loading different plugins and calling the
callbacks from the plugins to generate a new source code that would comply with
the needs of the benchmarking. In addition, the TFA core will generate a script that
describes actions of binary required to generate the needed benchmarking
information.

## TFA Tree view

tfa_core \
	├── inc \
	│   ├── tfa 	--> Public interface that can be accessed by a Plugin \
	│   └── tfa_pr  --> Private interface, only used by the shadow-builder \
	└── src 	--> Source files of the tfa module

## TFA Public Interface and Framework rules

In order to implement a plugin it is mandatory to implement two interface classes:

**IPlugin**: Two functions need to be implemented :

 * _virtual bool initializePlugin()_: This is the function, which
   initializes the plugin.
 
 * _virtual TFAInfoPlugin& getInfoPlugins()_: This is the function, which provides a list _TFAInfoPlugin_
   that tells what kind of plugin it is, what platform it supports,
   etc....

**ITFACommentListener**: This is a comment listener interface. The comment to listen to
is a tag. The tag needs to follow the pattern as described in the main
[README](../README.md###tags) file. From this class, the plugin must
implement the below functions:

 * _virtual Status runnableComments(const TFACommentInfo& comment, std::string& replacement)_:
   This function will be called during dispatching to match the string, that
   was provided during the creation of the ITFACommentListener.

 * In the constructor of the derived class, the derived class must call the
   constructor of ITFACommentListener with the string that the plugin wants to
   respond to.

Each **ITFACommentListener** implemented class needs to register itself in its plugin
by adding it to the _iclVect_, which is a protected member of the _IPlugin_ class.

Additionally, the  plugin must allocate a new _TFABenchMarkingPlatform(...)_ object  to
get additional information regarding the platform that is being supported.

The _TFABenchMarkingPlatform_ is an object, which needs 4 elements during its
creation: 

 * _std::string& OSName_: The name of the OS (i.e. "Linux, Nuttx, et.c ...)
 * _std::string& CPUArch_: The architecture of the platform i.e. "X86_64, ARM, etc ... 
 * _std::string& CPUName_: The CPU name, CPU model i.e "i7700K, arm-cortex-m4", etc ...
 * _std::string& boardName_: The board on which it runs i.e. "olimex, lauchpad, SOM, etc ...

Note that in case any of these parameters starts with a "\*", then it
means **do not care** and is ignored. This way this parameter is not checked and the plugin can
proceed.

## TFA Behavior

### During initialisation

During the shadow-builder initialisation, the TFA core will look for all
plugins. A plugin that has inherited from the _IPlugin_, as described in the
section [TFA Public Interface and Framework rules](##tfa-public-interface-and-framework-rules),
will be called by the core and the implemented function _initializePlugin()_  will
be called. Afterwards, the function _getInfoPlugin_ will be checked to get the name
of the plugin.



**Important Note**: The TFA core expects the plugin to register the list
of listeners in the _initializePlugin_ in its _iclVect_ vector, as well as, to
allocate the platform (_TFABenchMarkingPlatform\*_) in the _tbp_, which is inherited. An
example is shown below:

``` cpp
/* Here just allocating the platform */
	tbp = new TFABenchMarkingPlatform("Nuttx", "arm-m4", "*", "*");

/* Here adding listener element to the vector */
	ICLVect.emplace_back(static_cast<ITFACommentListener *>(new TFACMMonitorVar));
```

### While parsing code

This section describes how the TFA core behaves during the runtime. 

The TFA core receives a comment (not yet knowning if it is a **Tag** or not).
If the comment starts with `/** Bechmarking::`, the TFA core considers it as a **Tag**.

The tag is proceessed, which means that spaces are trimmed, parameters are extracted and comment delimiters are removed.

Once the tag is processed, the TFA core loops all the possible plugins, which are available.

Then, the TFA core will look at all registered elements in all plugins' _iclVect_.

If any plugin has registered a match in the _iclVect_, then it is passed to the plugin.

Then, the plugin processes the **Tag** received. Each time a plugin is not interested in the **Tag**,
it should respond returning the *Status::returnStatusError()*. This way, the TFA core 
continues to loop through the remaining plugins. 

If a plugin is interested in the **Tag**, it will fill the replacement and then it
shall return *Status::returnStatusOkay()*. This way the TFA core stops looping and returns to the file rewriter and parser to continue looking for the next
**Tag**.
