# WireGuard Implementation for ESP-IDF

This is an implementation of the [WireGuard&reg;](https://www.wireguard.com/)
for ESP-IDF.

[![Build examples](https://github.com/ciniml/wireguard-lwip-esp32/actions/workflows/build.yml/badge.svg)](https://github.com/ciniml/wireguard-lwip-esp32/actions/workflows/build.yml)

Almost all of this code is based on the
[WireGuard Implementation for lwIP](https://github.com/smartalock/wireguard-lwip),
but some potion of the code is adjusted to build with ESP- to run on ESP32
devices.

## Supported ESP-IDF versions and targets

The following ESP-IDF versions are supported:

* `master`
* `v4.2.2`
* `v4.3.1`

The following targets are supported:

* `esp32`
* `esp32s2`

## License

The original WireGuard implementation for lwIP is licensed under BSD 3 clause
license so the code in this repository also licensed under the same license.

Original license is below:

The code is copyrighted under BSD 3 clause Copyright (c) 2021 Daniel Hope
(www.floorsense.nz)

See [LICENSE](LICENSE) for details.
