#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

class OrymyBassAudioProcessorEditor : public juce::AudioProcessorEditor,
                                      public juce::Timer {
public:
    OrymyBassAudioProcessorEditor(OrymyBassAudioProcessor&);
    ~OrymyBassAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    OrymyBassAudioProcessor& audioProcessor;

    jucie::TextButton savePresetButton { "Save Preset" };
    juce::ComboBox presetBox;
    
    // Ancho y alto estandarizados basados en la referencia visual
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OrymyBassAudioProcessorEditor)
};
