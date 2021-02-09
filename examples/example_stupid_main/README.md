Example stupid main
===================

# Target

This example aims to show some simple implementation of a timer and
variable monitoring. 

These elements are necessary to present how the shadow-builder can be used to
benchmark a piece of a program. These are:

* A basic benchmark to measure the time spent to perform a simple loop
* A basic tracing element to display the value of a variable.

# Plugins involved

The only plugin involved in this example is located in
[tfa-plugin/plugin_test](../../tfa-plugins/plugin_test/README.md)

# How the shadow-builder is used?

Well, looking at the code, some piece of code is added to the original code:

``` C
/** Benchmarking::User::Timer(declare) */
/** Benchmarking::User::Monitor(declare) */
```

The above are the _"tags"_ that _"declare"_ the use of the Benchmarking timer and
variable monitor. They are mandatory as the plugin related documentation points it
out.

The syntax of any of benchmarking Tags have to start by using
**Benchmarking::User:Timer** or **Benchmarking::User:Monitor**. 
If is not followed thoroughly, the plugin expected to be used (i.e. plugin_test)
will not respond and, therefore, the plugin will not be answering.

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

Those are the _"tags"_ that actually _"execute"_ specific functions. In
this code we have: 
 * Benchmarking::User::Timer(start,timer1), 
 * Benchmarking::User::Monitor(U32, i) 2 times
 * Benchmarking::User::Timer(stop,timer1), 

These calls are decribed one by one:

### Benchmarking::User::Timer(start,timer1)

According to the plugin_test's documentation, the way to start the timer is to call
the following tag with the parameters **start** and **timer1**.
This way once the code is run, a timer will be created and started.

### Benchmarking::User::Monitor(U32, i)

This tag provides a way to monitor a specific variable, according to the
plugin_test's documetation. The first parameter **U32** means that the output will be
displayed as **U32**. The second parameter **i** means that the variable to monitor is
**i**. 
