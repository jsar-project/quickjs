# ⚡️ QuickJS - A mighty JavaScript engine

## Overview

QuickJS is a small and embeddable JavaScript engine. It aims to support the latest
[ECMAScript] specification.

This project is a _fork_ of the [original QuickJS project] by Fabrice Bellard and Charlie Gordon, after it went dormant, with the intent of reigniting its development.

## Getting started

Head over to the [project website] for instructions on how to get started and more
documentation.

## Zephyr

QuickJS can use Zephyr's libc and native synchronization primitives, including 64-bit Atomics on
targets without lock-free 64-bit instructions. The [Zephyr example](examples/zephyr/README.md)
builds and runs the engine directly on the `mps2/an385` QEMU board.

## Authors

[@bnoordhuis], [@saghul], and many more [contributors].

[ECMAScript]: https://tc39.es/ecma262/
[original QuickJS project]: https://bellard.org/quickjs
[@bnoordhuis]: https://github.com/bnoordhuis
[@saghul]: https://github.com/saghul
[contributors]: https://github.com/quickjs-ng/quickjs/graphs/contributors
[project website]: https://quickjs-ng.github.io/quickjs/
