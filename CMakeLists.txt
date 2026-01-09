cmake_minimum_required(VERSION 3.24)

# Project metadata
project(Pruvulazzu VERSION 0.1.0 LANGUAGES C CXX)

# Enable C++23 for latest features (std::expected, std::span improvements)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GTK3 on Linux for JUCE GUI support
if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    
    # IMPORTED_TARGET is required for each module to create the PkgConfig::<PREFIX> target
    pkg_check_modules(CURL REQUIRED IMPORTED_TARGET libcurl)
    pkg_check_modules(WEBKIT REQUIRED IMPORTED_TARGET webkit2gtk-4.1)
    pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0)
    pkg_check_modules(ALSA REQUIRED IMPORTED_TARGET alsa)
    pkg_check_modules(FREETYPE REQUIRED IMPORTED_TARGET freetype2)
    
    message(STATUS "WebKitGTK found: ${WEBKIT_VERSION}")
    message(STATUS "GTK3 found: ${GTK3_VERSION}")
    message(STATUS "ALSA found: ${ALSA_VERSION}")
    message(STATUS "Freetype found: ${FREETYPE_VERSION}")
    message(STATUS "cURL found: ${CURL_VERSION}")
endif()

# 1. Fetch JUCE
include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG origin/develop # Or a specific stable version
)
FetchContent_MakeAvailable(juce)

# 2. Create the Plugin
juce_add_plugin(Pruvulazzu
    COMPANY_NAME "josura"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    # Formats: VST3 and LV2 (Standard in JUCE 7+)
    # Standalone format to have an easy way to test the plugin (creates an executable)
    FORMATS VST3 LV2 Standalone
    PRODUCT_NAME "Pruvulazzu"
    LV2URI "https://josura.com/plugins/pruvulazzu" # To be changed later to actual URI 
)

target_compile_definitions(Pruvulazzu PRIVATE
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# 3. Source Files
target_sources(Pruvulazzu PRIVATE
    src/PluginProcessor.hxx
    src/PluginProcessor.cxx
    src/PluginEditor.hxx
    src/PluginEditor.cxx
)

# 4. Links and Dependencies
target_link_libraries(Pruvulazzu PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_gui_basics
    juce::juce_gui_extra    # it's required for file choosers/dialogs
    juce::juce_dsp
)

# Link Linux-specific system libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(Pruvulazzu PRIVATE
        PkgConfig::WEBKIT
        PkgConfig::GTK3 
        PkgConfig::ALSA 
        PkgConfig::FREETYPE
        PkgConfig::CURL
    )
endif()

# 5. Optimization Flags for High Performance
if(MSVC)
    target_compile_options(Pruvulazzu PRIVATE /O2 /arch:AVX2)
else()
    target_compile_options(Pruvulazzu PRIVATE -O3 -march=native -ffast-math)
endif()