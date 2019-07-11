Example stupid main
===================

# Target

This examples aims at showing some simple implementation of one of a timer and
the monitoring of a variable. 

This elements are necessary to display how the shadow-builder can be used to
benchmark some element of a program. Those are:

* A basic benchmarking measuring the time spent for the simple loop to perform
* A basic tracing element to show the value of a variable.

# Plugins involved

The only plugin involve atm for this example is located in
[tfa-plugin/plugin_test](../../tfa-plugins/plugin_test/README.md)

# How the shadow-builder is used?

Well, looking at the code, some piece of code are added to the original code:

``` C
/** Benchmarking::User::Timer(declare) */
/** Benchmarking::User::Monitor(declare) */
```

Those are the the _"tags"_ that _"declare"_ the use of the Benchmarking timer and
variable monitor. They are mandatory as the documentation fo the plugin points it
out.

The syntax of any of benchmarking Tags have to start by using
**Benchmarking::User:Timer** or **Benchmarking::User:Monitor**. 
If is not followed thoroughly, the plugin expected to be used (i.e. plugin_test)
will not respond and therefore the plugin will not be answering.

Then in the example's main function, there are 4 calls to the plugin.


``` C
	/** Benchmarking::User::Timer(start,timer1) */

	/** Benchmarking::User::Monitor(U32, i) */
	for (; i < UINT_MAX; i++) {
		data[i % sizeof(data)] = i % ((1 << (sizeof(char) * 8)) - 1);
	}

	/** Benchmarking::User::Monitor(U32, i) */

	/** Benchmarking::User::Timer(stop,timer1) */
```

Those are the the _"tags"_ that actually _"execute"_ specific functions. In
this code we have 
 * Benchmarking::User::Timer(start,timer1), 
 * Benchmarking::User::Monitor(U32, i) 2 times
 * Benchmarking::User::Timer(stop,timer1), 

Those call are decribed one by one:

### Benchmarking::User::Timer(start,timer1)

According to the plugin_test's documentation, the way to start the timer is call
the following tag with the parameters start and **timer1**.
This way once the code will be running, a timer will be created and started

### Benchmarking::User::Monitor(U32, i)

This tag provides a way to monitor a specific variable, according to the
plugin_test's documetation, the fist parameter U32 means that the output will be
displayed as U32. The second parameter "i" means that the variable to monitor is
"i" 
