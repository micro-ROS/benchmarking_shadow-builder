TFA Plugins
===========


# Getting Started

Writing a TFA plugin is not very complicated. Two ways of getting started are explained below.
But first, a general information regarding the way a plugin is working and how
to implement it in the simplest way:

### Create a plugin, what is needed
The shadow-builder is relying on TFA's plugins to be executed to answer the
parser dispatch. Therefore, the need of some interoperability is needed.

Every new plugin is written by implementing the **IPlugin** interface as shown in
the file `src_root/tfa_core/inc/tfa/IPlugin.h`. All the interface needs to do is to
implement the pure virtual functions. A simple example would be as in the
_plugin_test_.

In the plugin header:

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
In the plugin source code:

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

The above code shows a simple implementation of a plugin with two listeners.

## Simple way

### Generating the source

To ease the burden of newcomers, an automated way of providing an 
empty plugin as a mockup may be handy.

The execution of the script will create a base plugin project that can be later filled.

``` bash
root_src/res/tools/plugin_creator/create_plugin.sh  PluginTest
```

This script will basically create the folder `root_src/tfa-plugins/PluginTest`. After this the user might need to implement its listener.

In addition to listening to a specific comment, the plugin must implement the **ITFACommentListener** interface available at `src_root/tfa_core/inc/tfa/ITFACommentListener.h`.

All the details needed to do such implementation are described in the readme file 
[tfa_core/README.md](../tfa_core/README.md) .

## The hard way

This is not as hard as it sounds. In order to create a plugin, the list of
guidelines can be found here [tfa_core/README.md](../tfa_core/README.md).
Additionnaly, the binary output produced should be a shared library.

Binary output, which is produced, should be a shared library.

To compile it, a valid _CMakeLists.txt_ should be created. Some examples are available that can help you understand how to do it.

The remaining task is up to the developer.
Some guidelines and must-dos are explained in the 
[tfa_core/README.md](../tfa_core/README.md)

## Compilation

In order to start using the plugin, it's necessary to compile it. After all, the plugin is 
just a simple library object. To do so it is needed to add to the file 
`src_root/tfa-plugins/CMakeLists.txt` the line :

```
add_subdirectory(PluginTest) 
```

Once the subdirectory is added to the _CMakeLists.txt_ file, to compile the plugin
go into the `build` folder where shadow-builder is compiled and
type:

```bash
$ make
```

# Examples

<!-- A list of example/template are available in the tfa-plugins/ : -->
Some example templates are available in the `tfa-plugins` folder:
 * _plugin_test_: This is a simple implementation
 * _myplugin_: The HelloWorld plugin,
 * _plugin_template_: This is a template of what should be implemented.
