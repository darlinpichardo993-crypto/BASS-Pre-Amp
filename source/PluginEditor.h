#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <array>
#include <memory>

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
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    OrymyBassAudioProcessor& audioProcessor;
    juce::ComboBox presetBox;
    juce::Label presetLabel;
    std::array<juce::Slider, 6> controls;
    std::array<juce::Label, 6> controlLabels;
    std::array<std::unique_ptr<SliderAttachment>, 6> attachments;

    void applyPreset(int presetId);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OrymyBassAudioProcessorEditor)
};
