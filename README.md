Shadow Builder and Trace Framework Abstraction
============================================

The Trace Framework Abstraction is a module that will be a part of the shadow
builder. This module is in charge of abstracting the underlying tracing tool
used to benchmark. The tool is still a work in progress 
(everything can always be improved). So over the time new features and bug fixes will
be pushed.

# Table of Contents

1. [Concepts](#concepts)
2. [Architecture](#architecture)
3. [Getting started](#getting-started)
4. [Tested platforms](#tested_platform)
5.  [Common errors and bugs](#common-errors-and-bugs)

# Concepts

There is some information to get to know before jumping into the shadow builder. The
architecture section provides a glampse of the tool's guts. But, before we start, we need some
terminology to speak the same language.

## Terminology

### Tags
 A tag is a comment that is used by the shadow-builder. A comment is formatted
as follows:

 * _/\*\* Benchmarking::ModuleGroup::Function \*/_ : without any parameters,
 * _/\*\* Benchmarking::ModuleGroup::Function(param0,...,paramN) \*/_: with
   paramaters

and is considered a `shadow-builder comment` also known as a **Tag**.

### TFA: Trace Framework Abstraction  

This is a framework provided to help deloveloper to create various plugins.

### Plugins

Plugins are library files that provide replacement for a **Tag**.

### Parser

This element (front-end) is in charge of looking for a comment within a string. If
this string comment is a **Tag**, it is dispatched.

# Architecture

The Shadow-Builder is made of 4 core components that are:

 * **The parser element**: This is just a backend tool, which indicates the TFA
   module that a **Tag** was found in the source code. It's also in charge of
   writing some piece of code. Currently, the _llvm_ and _clang_ are used to parse and adapt the code. The use of _clang_ libtooling is documented more detailed
   on the official webpage: [https://releases.llvm.org/10.0.0/tools/clang/docs/index.html](https://releases.llvm.org/10.0.0/tools/clang/docs/index.html) .

 * **The TFA module**: This software is in charge of manipulating different 
   plugins and dispatching the **Tags** found by the parser
   element. It will perform the first filtering to make sure the comment is 
   formatted correctly. In addition, it also sanitises the commentaries (also
   called tags). The TFA module undersanding is quite straight forward, but more details are provided
   within the [tfa_core/README.md](tfa_core/README.md) .

 * **The TFA plugins**: These are the elements, that answer to a TFA module dispatch,
   when a commentary matches the commentaries. The plugins are basically a
   shared library file, that will be opened on startup. Some more details how to
   use and write them may be found here: [tfa-plugins/README.md](tfa-plugins/README.md) .

 * **Shadow Builder**: The umbrella core module that is in charge of
   orchestrating the modules aforementioned. More about it may be checked here: 
   [shadow-builder/README.md](shadow-builder/README.md) . 

## Tree

The tree view of the important folders, files and their descriptions is shown below.

benchmarking_shadow-builder\
	├── common  		--> Toolbox source files \
	├── examples		--> Examples of instrumented code \
	├── ext			--> External dl and libraries (do not pollute your machine and no need for docker) \
	├── prepare_build.sh    --> Script fetching and installing the dependencies \
	├── res			--> Configuration files folder \
	├── shadow-builder	--> Shadowbuilder source files \
	├── tfa_core		--> TFA's source files, \
	└── tfa-plugins		--> TFA plugin's folder.

# Getting started

## Dependencies

First of all, in order to start compiling the shadow-builder, it is needed to
retrieve a list of dependencies. 

### Under Ubuntu 18.04:

``` shell
sudo apt install git libzmq3-dev binutils libssl-dev python3-distutils python3
sudo snap install cmake --classic # To get a more recent version of cmake.
```
Make sure the correct version of _cmake_ is being applied. 

``` shell
cmake --version
```

In order to build _clang_ and _LLVM_, gcc 9 is needed. Under the official ppa, this
is not possible to find it. Therefore, we will need to install it from a test ppa:

``` shell
sudo add-apt-repository ppa:ubuntu-toolchain-r/test # Accept the key by pressing enter
sudo apt update
sudo apt install gcc-9 g++-9 g++-9-multilib
```

Then, once this is done, as you might already have a gcc on your machine, the 
update-alternative should be used to have different version of gcc. If the
alternative for gcc is available and set up on your machine, you can directly skip the
command block below.

``` shell
sudo update-alternatives --remove-all gcc
sudo update-alternatives --remove-all cc
sudo update-alternatives --remove-all g++
sudo update-alternatives --remove-all c++

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-xxx 10 # This is your gcc compiler
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 20

sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-xxx 10 # This is your g++ compiler
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 20

sudo update-alternatives --install /usr/bin/cc cc /usr/bin/gcc 30
sudo update-alternatives --set cc /usr/bin/gcc

sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 30
sudo update-alternatives --set c++ /usr/bin/g++

sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 30
sudo update-alternatives --set c++ /usr/bin/g++
``` 

Then, select the gcc9 and g++9 as follows:

``` shell
sudo update-alternatives --config gcc
There are 2 choices for the alternative gcc (providing /usr/bin/gcc).

  Selection    Path            Priority   Status
------------------------------------------------------------
* 0            /usr/bin/gcc-9   20        auto mode
  1            /usr/bin/gcc-7   10        manual mode
  2            /usr/bin/gcc-9   20        manual mode

Press <enter> to keep the current choice[*], or type selection number: 2
```

And repeat these steps for:
 * c++ --> selecting /usr/bin/g++
 * cc --> selecting /usr/bin/gcc
 * gcc --> selecting /usr/bin/gcc-9
 * g++ --> selecting /usr/bin/g++-9


It is possible to check the right version of gcc and g++ in the command line:

``` shell
gcc -v
g++ -v
cc -v
c++ -v
```


If some dependencies result to be missed, please feel free to add them into this
document and then initiate a pull request.

## Compilation 

Before starting, some internal dependencies must be retrieved. To do so, the
script at the root folder should be called as follow:

``` shell
./prepare_build.sh # retrieves/compiles and installs dependencies
```

Once this step is done, you can delete the `ext/dl` subdirectory. This will save you a lot
of space.

**This might take some time, so grab yourself a coffee.**


Then, once all dependencies are built, the _shadow-builder_ is ready to be
compiled:

``` shell
mkdir -p build; cd build
cmake ..
make -j4
```

## First run: using the example/example_stupid_main

To run the shadow builder against the examples (located in the examples/example_stupid_main),
the following steps are to be performed:

```shell
cd build # previously created during the compilation step.
./shadow-program -s ../res/sb-res/bcf.xml -t ../res/tfa-res/tfa.xml
```

This command will create an instrumented code that will be generated in the
folder `/tmp/output/test_DATE`. 

The following commands will compile and execute the code:

``` shell
cd /tmp/output/test_DATE/
mkdir -p build; cd build
cmake .. ; make
./simple_stupid_example 
```

The output of the last command should be like this:

``` shell
Monitor var i: 0
Monitor var i: 4294967295
Exe time: 8 sec : 758 ms : 534352 ns
```

To understand what is going on, it is recommended to go through all configuration
files' explainations and readmes, the codes' examples and their readmes. 

It is good to start with all readmes to get more details. The code is
documentated as well and can provide additional information at a very detailed level.

## Configuration files

The shadow-builder uses XML configuration files to locate the TFA plugin folders,  
**currently the only one folder that can be provided**. By default the plugin folder will
be located in the folder `<src_root>/tfa-plugins/`.

More information regarding the different attributes and nodes for the XML
configuraition can be found here: [res/README.md](res/README.md) .

## Plugins

Plugins are the **Responders** to **Tags** that are found in the code. These **Tags**
are used to tell the shadow-builder, and more specifically its plugins, that a
benchmarking request is made in the code. 

The response is up to the implementation of the plugin. For more details, please
have a look here [tfa-plugins/README.md](tfa-plugins/README.md) .


## Unit tests

Unit tests are available in the module subfolders `tests`.
Each test cpp file shall be added to the `CMakeFile.txt` in the module you are testing.

In addition, the option -DENABLE_TESTS=ON shall be passed to the cmake to
activate tests creation.

Currently a few tests are performed. In the future, more unit tests will
be added.

# Tested Platforms

The shadow-builder was successfully compiled with ubuntu 18.04 LTS with the following
configuration:

 * 2 Core / 4 Threads Intel Processor
 * 4GB DDR4 RAM / 8 GB of swapping memory,
 * 60 GB of disk space.

# Common errors and bugs

## Not enough Disk space

Compilations of _LLVM_ and _clang_ produce a lot of object files.
During their preparation it is needed around 36GB of disk space.

## Not enough RAM during LLVM/clang compilation

This error might occur during the execution of the script _prepare_build.sh_.

Unfortunately, the _LLVM/clang_ compilation is quite greedy when it comes to resources needed
to compile it.

This error arises due to a low RAM. Currently, the only way to solve it, would be to increase
the swap size as explained here [https://askubuntu.com/questions/927854/how-do-i-increase-the-size-of-swapfile-without-removing-it-in-the-terminal](https://askubuntu.com/questions/927854/how-do-i-increase-the-size-of-swapfile-without-removing-it-in-the-terminal)

# LICENSE

The Shadow Builder tool suite (which includes the TFA and Shadow-Builder) is to be open-sourced under the Apache-2.0 license. See the
[LICENSE](./LICENSE) file for details.

For a list of other open-source components included in the Shadow Builder tool suite (which includes the TFA and Shadow-Builder), see
the file [3rd-party-licenses.md](./3rd-party-licenses.md).
