# Aeolus
Granular sampler plugin implemented in C++ and Juce. Support both LV2 and VST. Under production

## Project Structure
```
Aeolus/
├── CMakeLists.txt        # Main build script
├── src/               # All C++ code
│   ├── PluginProcessor.cpp
│   ├── PluginProcessor.h
│   ├── PluginEditor.cpp
│   ├── PluginEditor.h
│   └── DSP/              # Grain logic, buffers, etc.
├── tests/                # Unit tests (Catch2 or GTest)
├── modules/              # Git submodules (like JUCE)
└── resources/            # Icons, fonts, and assets
```

## Requirements
- CMake 3.15+
- A C++23 compatible compiler (GCC 11+, Clang 12+, MSVC 2019+)
- gtk 3 (Linux), and webkit2gtk (for embedded browser, Linux)
- ALSA development libraries (Linux)
- freetype2 development libraries (Linux)
- sound2 development libraries
- curl

## Building
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
make
```