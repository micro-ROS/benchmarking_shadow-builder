TFA Plugins
===========


# Getting Started

Writing a TFA plugin is not very complicated. Two of starting with it are explained below.
But first, a general information regarding the way a plugin is working and how
to implement it in the simplest way:

### Create a plugin, what is needed
The shadow-builder is relying on tfa's plugins to be executed to answer the
parser dispatch. Therefore the need of some interoperability is needed.

Every new plugins are written by implementing the IPlugin interface as shown in
the file src_root/tfa_core/inc/tfa/IPlugin.h. All the interface needs to do is to
implement the pure virtual function. A simple example would be as in the
plugin_test:

in the plugin header:

``` cpp
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
``` 
in the plugin source code:

``` cpp
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
```

The above code show a simple implementation of a plugin with two listeners.

## Simple way

### Generating the source

In order to ease the burden of new comers, an _automated_ way of providing an 
empty plugin that has to be mocked.

The execution of the script will create a base project plugin that can be filled.

``` bash
root_src/res/tools/plugin_creator/create_plugin.sh  PluginTest
```

This script willl basically create a folder in the folder root_src/tfa-plugins/PluginTest
After this the user might need to implement it's listener.

Additionally to listen to a specific comment the plugin must implement of a
the ITFACommentListener available src_root/tfa_core/inc/tfa/ITFACommentListener.h

All information needed regarding those implementation is described in the 
[tfa_core/README.md](../tfa_core/README.md) .

## The hard way

This is not as hard as it sounds. In order to create a plugin, the list of
guideline as explained here [tfa_core/README.md](../tfa_core/README.md).
Additionnaly, the output produced binary should be shared library.

To compile it, a valid CMakeLists.txt should be created. Examples are available to
help undestand how to create its own CMakeLists.txt

The remaining task is up to the developer.
Some guideline and must-dos are explained in the 
[tfa_core/README.md](../tfa_core/README.md)

## Compilation

In order to start using the plugin, it's necessary to compile it. After all, the plugin is 
just a simple library object. To do so it's needed to add to a the file located in 
src_root/tfa-plugins/CMakeLists.txt the line :

```
add_subdirectory(PluginTest) 
```

Once the subdirectory added toe the CMakeLists.txt file, to compile the plugin
is done by going in the **build** folder where is compiled shadow-builder and
type:

```bash
$ make
```

# Examples

A list of example/template are available in the tfa-plugins/ :

 * The plugin_test : This is a simple implementation
 * The myplugin : A simple HelloWorld plugin,
 * The plugin_template : This is a template of what should be implemented.
