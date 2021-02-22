Example Stupid Main
===================

# Target

This example aims to show simple implementation of monitoring the elapsed time and value of a  variable. It is composed of two elements:

* the basic benchmark to measure the time spent doing a simple loop,
* the basic tracker for displaying the value of a variable.

These elements are helpful to present how the shadow-builder can be used to benchmark a piece of a program code.

# Plugins involved

The only [plugin involved in this example](../../tfa-plugins/plugin_test/README.md) is located in the folder `tfa-plugins/plugin_test`.

# How the shadow-builder is used

Well, looking at the code, some piece of code is added to the original code:

``` C
/** Benchmarking::User::Timer(declare) */
/** Benchmarking::User::Monitor(declare) */
```

The above are the `tags` that `declare` benchmarking of the timer and
variable monitor. They are mandatory as the plugin related documentation points it
out.

The syntax of any of benchmarking tags must begin with 
`Benchmarking::User:Timer` or `Benchmarking::User:Monitor`. If it is not followed thoroughly, the plugin expected to be used (i.e. _plugin_test_`)
will not respond and, therefore, the plugin will not be answering.

Then, in the example's main function, there are 4 calls to the plugin.


``` C
	/** Benchmarking::User::Timer(start,timer1) */

	/** Benchmarking::User::Monitor(U32, i) */
	for (; i < UINT_MAX; i++) {
		data[i % sizeof(data)] = i % ((1 << (sizeof(char) * 8)) - 1);
	}

	/** Benchmarking::User::Monitor(U32, i) */

	/** Benchmarking::User::Timer(stop,timer1) */
```

These calls are the `tags` that actually `execute` specific functions. In
this code we have: 
 * _Benchmarking::User::Timer(start,timer1)_, 
 * _Benchmarking::User::Monitor(U32, i)_ which is called two times,
 * _Benchmarking::User::Timer(stop,timer1)_. 

Above calls are decribed one by one:

### Benchmarking::User::Timer(start,timer1)

According to the _plugin_test's_ documentation, the way to start the timer is to call
the tag with the parameters `start` and `timer1`.
This way once the code is run, a timer will be created and started.

### Benchmarking::User::Monitor(U32, i)

This tag provides a way to monitor a specific variable as it can be found in the
_plugin_test's_ documetation. The first parameter `U32` means that the output will be
displayed as a 32-bit unsigned integer number. The second parameter `i` means that the variable to monitor is `i`. 

### Benchmarking::User::Timer(stop,timer1)

To stop a timer created and started previously, the tag with the parameters `stop` and `timer1`should be called. Once the code is run, the timer1 will be stopped.
