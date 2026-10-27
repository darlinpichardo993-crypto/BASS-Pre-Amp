cmake_minimum_required(VERSION 3.22)
project(OrymyBass VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(OrymyBass
    COMPANY_NAME "YMY Sounds VST"
    BUNDLE_ID "com.ymysounds.orymybass"
    PLUGIN_NAME "ORYMY Bass"
    PLUGIN_MANUFACTURER_CODE YmyS
    PLUGIN_CODE OrBs
    FORMATS VST3 Standalone
    PRODUCT_NAME "ORYMY Bass"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(OrymyBass PRIVATE
    PluginProcessor.cpp
    PluginProcessor.h
    source/PluginEditor.cpp
    source/PluginEditor.h
)

target_include_directories(OrymyBass PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/source"
)

target_compile_definitions(OrymyBass PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(OrymyBass PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_extra
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)
