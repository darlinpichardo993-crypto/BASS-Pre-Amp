#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

class OrymyBassAudioProcessor;

class OrymyBassAudioProcessorEditor : public juce::AudioProcessorEditor,
                                      private juce::Timer
{
public:
    explicit OrymyBassAudioProcessorEditor(OrymyBassAudioProcessor&);
    ~OrymyBassAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    OrymyBassAudioProcessor& audioProcessor;
    juce::ComboBox presetBox;
    juce::Label presetLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OrymyBassAudioProcessorEditor)
};
