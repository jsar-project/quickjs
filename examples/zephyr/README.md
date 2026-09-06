# Zephyr example

This sample builds the QuickJS engine directly into a Zephyr application and validates the C API,
promises and pending jobs, SharedArrayBuffer, timed waits, and 8/16/32/64-bit Atomics.

Zephyr 4.4.2 and SDK 1.0.1 were used for the initial port. The `mps2/an385` QEMU board has enough
RAM and flash for the full engine.

```sh
source /Users/yorkie/zephyrproject/env.sh
cd /Users/yorkie/zephyrproject
west build -b mps2/an385 /Users/yorkie/workspace/jsar-project/quickjs/examples/zephyr \
  -d build/quickjs-zephyr
west build -d build/quickjs-zephyr -t run
```

Exit QEMU with Ctrl+A, then X.
