# Inactive

**Old description**

Project aims to implement a C-like language interpreter with small code and memory footprint for 32bit microcontrollers.

At the time of writing, parser code size for Cortex-M3 is less than 10K and VM is less than 3K. RAM requirements mostly depend on code size and complexity, but few kilobytes should be enough to handle most reasonably simple cases.

Parser and VM are written with code size priority, not security. Project should not be used to run code (and bytecode) from untrusted sources.
