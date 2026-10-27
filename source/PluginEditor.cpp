#include "PluginEditor.h"
#include "PluginProcessor.h"

OrymyBassAudioProcessorEditor::OrymyBassAudioProcessorEditor(OrymyBassAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(1100, 720);
    setResizable(true, true);
    setResizeLimits(760, 500, 1600, 1000);

    presetLabel.setText("PRESET", juce::dontSendNotification);
    presetLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFE4D8C6));
    presetLabel.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::bold)));
    addAndMakeVisible(presetLabel);

    presetBox.addItem("Clean Bachata", 1);
    presetBox.addItem("Antony Santos - Warm", 2);
    presetBox.addItem("Raulin Rodriguez - Smooth", 3);
    presetBox.addItem("Romeo - Modern", 4);
    presetBox.addItem("Max Santos - Punch", 5);
    presetBox.addItem("Slap Bachata", 6);
    presetBox.addItem("Merengue - Tight", 7);
    presetBox.setSelectedId(1, juce::dontSendNotification);
    presetBox.setTooltip("Presets will be wired to parameter states in a later DSP/UI pass.");
    addAndMakeVisible(presetBox);
    startTimerHz(30);
}

OrymyBassAudioProcessorEditor::~OrymyBassAudioProcessorEditor()
{
    stopTimer();
}

void OrymyBassAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF141719));
    g.setColour(juce::Colour(0xFF22282B));
    g.fillRect(0, 0, getWidth(), 78);

    g.setColour(juce::Colour(0xFFC89B53));
    g.fillRect(0, 76, getWidth(), 2);

    g.setColour(juce::Colour(0xFFF4EEE3));
    g.setFont(juce::Font(juce::FontOptions(25.0f, juce::Font::bold)));
    g.drawText("ORYMY BASS", 24, 12, 300, 34, juce::Justification::left);

    g.setColour(juce::Colour(0xFFC89B53));
    g.setFont(juce::Font(juce::FontOptions(10.0f, juce::Font::bold)));
    g.drawText("YMY SOUNDS  /  BACHATA • MERENGUE", 26, 46, 380, 18, juce::Justification::left);

    g.setColour(juce::Colour(0xFF202629));
    g.fillRoundedRectangle(20.0f, 102.0f, static_cast<float>(getWidth() - 40), 190.0f, 12.0f);
    g.setColour(juce::Colour(0xFF3A4144));
    g.drawRoundedRectangle(20.0f, 102.0f, static_cast<float>(getWidth() - 40), 190.0f, 12.0f, 1.0f);

    g.setColour(juce::Colour(0xFFE4D8C6));
    g.setFont(juce::Font(juce::FontOptions(14.0f, juce::Font::bold)));
    g.drawText("PREAMP ENGINE", 40, 120, 260, 24, juce::Justification::left);

    g.setColour(juce::Colour(0xFF9BA5A8));
    g.setFont(juce::Font(juce::FontOptions(12.0f)));
    g.drawText("Input gain  •  amp saturation  •  master  •  output gain", 40, 152, getWidth() - 80, 24, juce::Justification::left);
    g.drawText("This initial interface is a buildable foundation; controls and preset switching still need full UI wiring.", 40, 188, getWidth() - 80, 42, juce::Justification::left);

    g.setColour(juce::Colour(0xFFC89B53));
    g.setFont(juce::Font(juce::FontOptions(11.0f, juce::Font::bold)));
    g.drawText("ORYMY BASS  •  VERSION 1.0.0", 24, getHeight() - 34, getWidth() - 48, 18, juce::Justification::centredLeft);
}

void OrymyBassAudioProcessorEditor::resized()
{
    presetLabel.setBounds(30, 315, 80, 28);
    presetBox.setBounds(105, 314, 270, 30);
}

void OrymyBassAudioProcessorEditor::timerCallback()
{
    // Reserved for future visual metering; keep audio processing out of the UI timer.
}
