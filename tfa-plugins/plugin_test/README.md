Plugin Test
===========


# General information

This plugin implements two benchmarking elements. The first one is helpful to measure the time spent in the selected segment of the code, the other one provides the way of reporting the value
of a designated variable.

# Target Platform

The plugin is aimed at all Linux OS.

# Tag comments

The plugin can answer to the following `tag comments`:
 * Benchmarking::User::Timer
 * Benchmarking::User::Monitor.

#  Benchmarking::User::Timer

This tag implements the time measurement. Basically, it relies on the _timespecs_ linux stucture and _clockgettime_ function 
to retrieve the time that was spent in the  measured section. The specifics of the tag's parameters are as follows:

 * _Benchmarking::User::Timer(declare)_: You have to add it at the very beginning
   of every file in which you want to benchmark time.
 * _Benchmarking::User::Timer(start, timer_name)_: This is used to start the timer called `timer_name`.
 * _Benchmarking::User::Timer(stop, timer_name)_: This is used to stop the timer called `timer_name`.

_Note_:
The `timer_name` should be the same for starting and stopping a timer when the elapsed time is being measured.

#  Benchmarking::User::Monitor

This tag implements reporting the value of a variable by calling
the `prinf` function from the `libc`library. Its parameters are used as follows:
  
 * _Benchmarking::User::Monitor(declare)_: You have to add it at the very beginning
   of every file in which you want to report any variable.

 * _Benchmarking::User::Monitor(type, variable)_: This is used to report the `variable` value. The displayed value is formatted on assumption of a variable type based on the provided `type` parameter.

The following types of variables are implemented: 

 * U32: 32-bit unsigned integer.

_Note_:
The variable of the variable parameter's name has to exist in the code.
