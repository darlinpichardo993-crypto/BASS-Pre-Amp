#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace
{
constexpr std::array<const char*, 6> parameterIds {
    "input_gain", "amp_gain", "amp_master", "drive", "mix", "output_gain"
};
constexpr std::array<const char*, 6> controlNames {
    "INPUT", "AMP GAIN", "MASTER", "DRIVE", "SAT MIX", "OUTPUT"
};
}

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
    presetBox.onChange = [this] { applyPreset(presetBox.getSelectedId()); };
    addAndMakeVisible(presetBox);

    for (size_t i = 0; i < controls.size(); ++i)
    {
        auto& slider = controls[i];
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 78, 22);
        slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFFC89B53));
        slider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF596165));
        slider.setColour(juce::Slider::thumbColourId, juce::Colour(0xFFF4EEE3));
        slider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFF4EEE3));
        slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF171B1D));
        slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF42494C));
        slider.setTooltip(controlNames[i]);
        addAndMakeVisible(slider);

        auto& label = controlLabels[i];
        label.setText(controlNames[i], juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setColour(juce::Label::textColourId, juce::Colour(0xFFE4D8C6));
        label.setFont(juce::Font(juce::FontOptions(11.0f, juce::Font::bold)));
        addAndMakeVisible(label);

        attachments[i] = std::make_unique<SliderAttachment>(audioProcessor.parameters, parameterIds[i], slider);
    }

    startTimerHz(30);
}

OrymyBassAudioProcessorEditor::~OrymyBassAudioProcessorEditor()
{
    stopTimer();
}

void OrymyBassAudioProcessorEditor::applyPreset(int presetId)
{
    struct Preset { float input, amp, master, drive, mix, output; };
    Preset preset { 0.0f, 5.0f, 7.0f, 1.0f, 65.0f, 0.0f };

    switch (presetId)
    {
        case 2: preset = { 1.0f, 5.7f, 7.2f, 1.4f, 72.0f, -1.0f }; break;
        case 3: preset = { 0.0f, 4.7f, 7.0f, 0.8f, 58.0f, 0.0f }; break;
        case 4: preset = { 1.0f, 6.0f, 6.8f, 1.8f, 78.0f, -1.5f }; break;
        case 5: preset = { 2.0f, 6.8f, 6.5f, 2.2f, 88.0f, -2.0f }; break;
        case 6: preset = { 0.0f, 7.0f, 6.0f, 3.2f, 100.0f, -3.0f }; break;
        case 7: preset = { 1.0f, 5.8f, 7.5f, 1.2f, 68.0f, -1.0f }; break;
        default: break;
    }

    const std::array<float, 6> values {
        preset.input, preset.amp, preset.master, preset.drive, preset.mix, preset.output
    };
    for (size_t i = 0; i < values.size(); ++i)
    {
        if (auto* parameter = audioProcessor.parameters.getParameter(parameterIds[i]))
            parameter->setValueNotifyingHost(parameter->convertTo0to1(values[i]));
    }
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
    g.fillRoundedRectangle(20.0f, 102.0f, static_cast<float>(getWidth() - 40), 435.0f, 12.0f);
    g.setColour(juce::Colour(0xFF3A4144));
    g.drawRoundedRectangle(20.0f, 102.0f, static_cast<float>(getWidth() - 40), 435.0f, 12.0f, 1.0f);

    g.setColour(juce::Colour(0xFFE4D8C6));
    g.setFont(juce::Font(juce::FontOptions(14.0f, juce::Font::bold)));
    g.drawText("PREAMP ENGINE", 38, 120, 260, 24, juce::Justification::left);

    g.setColour(juce::Colour(0xFF9BA5A8));
    g.setFont(juce::Font(juce::FontOptions(12.0f)));
    g.drawText("Analog-inspired saturation • parameter automation • preset snapshots", 38, 146, getWidth() - 76, 22, juce::Justification::left);

    g.setColour(juce::Colour(0xFFC89B53));
    g.setFont(juce::Font(juce::FontOptions(11.0f, juce::Font::bold)));
    g.drawText("ORYMY BASS  •  VERSION 1.0.0", 24, getHeight() - 34, getWidth() - 48, 18, juce::Justification::centredLeft);
}

void OrymyBassAudioProcessorEditor::resized()
{
    const int margin = 30;
    presetLabel.setBounds(margin, 82, 70, 26);
    presetBox.setBounds(margin + 72, 82, juce::jmin(300, getWidth() / 2), 28);

    const int usableWidth = getWidth() - margin * 2;
    const int columnWidth = usableWidth / static_cast<int>(controls.size());
    const int knobSize = juce::jmin(112, columnWidth - 8);
    const int knobY = 235;

    for (size_t i = 0; i < controls.size(); ++i)
    {
        const int x = margin + static_cast<int>(i) * columnWidth + (columnWidth - knobSize) / 2;
        controlLabels[i].setBounds(margin + static_cast<int>(i) * columnWidth, 195, columnWidth, 24);
        controls[i].setBounds(x, knobY, knobSize, knobSize + 30);
    }
}

void OrymyBassAudioProcessorEditor::timerCallback()
{
    // Keep audio processing on the audio thread; UI metering can be added separately.
}
