# Pruvulazzu
Granular sampler plugin implemented in C++ and Juce. Support both LV2 and VST. Under production

Drag and drop samples to play them granually with MIDI.

## Name origin
Pruvulazzu means "dust" or "small particles" in sicilian. The sicilian undertones of this plugin will probably be coded as well in the UI in the future.

## Project Structure
```
Pruvulazzu/
├── CMakeLists.txt        # Main build script
├── src/               # All C++ code
│   ├── PluginProcessor.cpp
│   ├── PluginProcessor.hxx
│   ├── PluginEditor.cpp
│   ├── PluginEditor.hxx
│   └── DSP/              # Grain logic, buffers, etc.
├── tests/                # Unit tests (GTest most probably)
└── resources/            # Icons, fonts, and assets, TODO be added
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

## TODO
- [ ] Add more DSP features (LFO, Envelopes, etc.)
- [ ] Improve UI/UX design
- [ ] Add more sample formats support
- [ ] Write unit tests
- [ ] Documentation
- [ ] Grain logic optimization and improvements
- [ ] Better visualization of grains in the waveform display (maybe small windows or clear markers)
