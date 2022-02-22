google-cloud-c
==============
[![CI for Linux, Windows, macOS](https://github.com/offscale/google-cloud-c/actions/workflows/linux-Windows-macOS.yml/badge.svg)](https://github.com/offscale/google-cloud-c/actions/workflows/linux-Windows-macOS.yml)
[![License](https://img.shields.io/badge/license-Apache--2.0%20OR%20MIT-blue.svg)](https://opensource.org/licenses/Apache-2.0)

Google Cloud Client library in C. TODO: code-generate the bulk of this library.

## vcpkg

[`vcpkg`](https://vcpkg.io) is an open-source cross-platform library package management system from [Microsoft](https://microsoft.com); targeting macOS, Linux, and Windows.

It's very popular, and has strong CMake integration. Henceforth, it is chosen for this project as an example of third-party library integration.

[Install vcpkg](https://vcpkg.io/en/getting-started.html), configure it with your system. Then run:

    [root]/vcpkg/vcpkg install curl

Or do the same with your system package manager, `conan`, or whatever else you use.

## Build

    $ mkdir 'cmake-build-debug' && cd "$_"
    $ cmake -DCMAKE_TOOLCHAIN_FILE='[root]/vcpkg/scripts/buildsystems/vcpkg.cmake' \
            -DCMAKE_BUILD_TYPE='Debug' \
            ..
    $ cmake --build .


## TODO

  - CTest
  - CPack
  - More docs
  - **Code-generate `struct`s, `free` code, and HTTPS calls**

---

## License

Licensed under either of

- Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE) or <https://www.apache.org/licenses/LICENSE-2.0>)
- MIT license ([LICENSE-MIT](LICENSE-MIT) or <https://opensource.org/licenses/MIT>)

at your option.

### Contribution

Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in the work by you, as defined in the Apache-2.0 license, shall be
dual licensed as above, without any additional terms or conditions.
