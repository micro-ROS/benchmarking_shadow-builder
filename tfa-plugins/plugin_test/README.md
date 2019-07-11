Plugin Test
===========


# General information

This plugin will provide an implemented way of measuring the time between two
part of the code as it will be described below.
Additionnaly, the plugin will provide a way of reporting/monitoring the value
of a designated variable.

# Target Platform

The plaform aimed for this tool are all Linux platform OS.

# Tag comments

The list of tag comment that this plugin will answer to:

 * Benchmarking::User::Timer
 * Benchmarking::User::Monitor

#  Benchmarking::User::Timer

This is the implementation of a timer benchmarking element.
Basically it relies on the timespecs linux stucture and clockgettime 
to retrieve the time that was spent in measured section.

This tag can have different parameters:

 * Benchmarking::User::Timer(declare): Need to be added at the very beginning by every
   file that will be using this tag
 * Benchmarking::User::Timer(start, **timer_name**): start a timer called **timer_name**,
 * Benchmarking::User::Timer(stop, **timer_name**): stop a timer called **timer_name**.

_Cautions_:
The timer_name should be the same for start and stop. **It has to be unique.**

#  Benchmarking::User::Monitor

This is implementing the monitoring of a variable to retrieve it's value using
the basic prinf function from the libc.

This tag casn have different parameters:

 * Benchmarking::User::Monitor(declare): Need to be added at the very beginning
   of every file using this element.
 * Benchmarking::User::Monitor(**type**,**variable**): This will print the **variable** to be
   used. The **type** is informing the plugin in the format that it should be
   displayed.

Regarding the format for the type of variable regarding the Monitor, the type
currently implemented are:

 * U32: Display as an unsigned 32 bits.
 * More will be added...

_Cautions_:
**The _variable_ parameter has to exists in the code!**
