# Build and test the OFX stub

Use a native macOS arm64 machine with AppleClang, or Windows x64 with Visual Studio 2022 and the v143 C++ toolset. Both require CMake 3.31 or later and C++17. The checkout includes the OpenFX headers and license, so configuration and builds do not need network access.

On macOS, run these commands from the repository root.

```sh
cmake -S . -B build/mac -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64
cmake --build build/mac --parallel
ctest --test-dir build/mac -C Release --output-on-failure --no-tests=error
```

On Windows, run these commands in PowerShell from the repository root.

```powershell
cmake -S . -B build/windows -G "Visual Studio 17 2022" -A x64 -T v143
cmake --build build/windows --config Release --parallel
ctest --test-dir build/windows -C Release --output-on-failure --no-tests=error
```

Each build produces `build/<directory>/bundle/Release/yorozu-fx.ofx.bundle`. The native binary is `Contents/MacOS/yorozu-fx.ofx` on macOS and `Contents/Win64/yorozu-fx.ofx` on Windows. Each bundle contains the unchanged OpenFX license under `Contents/Resources/openfx-LICENSE.md`. macOS also includes `Contents/Info.plist`.

CTest runs four checks. The ABI test loads the actual module and exercises its exported descriptor and callbacks. The missing-module test expects a nonzero loader exit. The package check compares the bundled license and validates the native path and macOS executable metadata. The SDK check compares vendored files with `third_party/openfx/SHA256SUMS`. All checks remain active in Release.

The stub exposes one provisional `org.yorozufx.buildstub` descriptor. It rejects host Load with `kOfxStatFailed`. Describe, Render, and unknown actions return `kOfxStatReplyDefault`. It contains no color engine and is not usable as a Resolve effect. Do not install the stub into Resolve.

CI downloads CMake 3.31.6 from Kitware and verifies fixed archive SHA256 hashes. It runs native builds and tests on `macos-15` arm64 and `windows-2022` x64, then uploads each exact bundle and CTest log. Action revisions are pinned. Runner labels and the v143 toolset do not pin compiler patch versions. Each job prints its actual image, compiler, CMake version, architecture, repository commit, and binary hash.

Local verification used CMake 4.4.3 and AppleClang 21.0.0 on arm64. This machine's Command Line Tools did not find its C++ standard headers by default. Local verification supplied `CXXFLAGS="-isystem $(xcrun --show-sdk-path)/usr/include/c++/v1"` only when configuring the local build directory. The project and CI contain no machine-specific include path. Native hosted CI remains required to verify the ordinary commands above.

These tests do not establish Resolve 20 or 21 compatibility, image output, Metal, CUDA, persistence, signing, or distribution readiness. P03 and later tasks own those checks.
