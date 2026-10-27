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
    setSize(1240, 790);
    setResizable(true, true);
    setResizeLimits(900, 650, 1700, 1100);

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
    const auto bg = juce::Colour(0xFF101315);
    const auto panel = juce::Colour(0xFF202426);
    const auto gold = juce::Colour(0xFFD6AA62);
    const auto cream = juce::Colour(0xFFF3E8D5);
    const auto muted = juce::Colour(0xFF899195);
    const float w = static_cast<float>(getWidth());
    const float h = static_cast<float>(getHeight());

    g.fillAll(bg);
    juce::ColourGradient metal(juce::Colour(0xFF343A3C), 0, 0,
                               juce::Colour(0xFF171B1D), w, h, false);
    g.setGradientFill(metal);
    g.fillRoundedRectangle(10.0f, 10.0f, w - 20.0f, h - 20.0f, 18.0f);

    // Machined faceplate and gold inset edge.
    g.setColour(juce::Colour(0xFF080A0B));
    g.drawRoundedRectangle(10.0f, 10.0f, w - 20.0f, h - 20.0f, 18.0f, 3.0f);
    g.setColour(juce::Colour(0xFF62686A));
    g.drawRoundedRectangle(17.0f, 17.0f, w - 34.0f, h - 34.0f, 13.0f, 1.0f);
    g.setColour(gold.withAlpha(0.8f));
    g.drawRoundedRectangle(22.0f, 22.0f, w - 44.0f, h - 44.0f, 10.0f, 1.0f);

    // Hardware screws.
    for (const auto point : { juce::Point<float>(38, 38), juce::Point<float>(w - 38, 38),
                              juce::Point<float>(38, h - 38), juce::Point<float>(w - 38, h - 38) })
    {
        g.setColour(juce::Colour(0xFF090B0C));
        g.fillEllipse(point.x - 7, point.y - 7, 14, 14);
        g.setColour(juce::Colour(0xFF777D7F));
        g.drawEllipse(point.x - 6, point.y - 6, 12, 12, 1.0f);
        g.drawLine(point.x - 3.5f, point.y + 3.5f, point.x + 3.5f, point.y - 3.5f, 1.2f);
    }

    // Header branding.
    g.setColour(cream);
    g.setFont(juce::Font(juce::FontOptions(30.0f, juce::Font::bold)));
    g.drawText("ORYMY", 52, 35, 190, 38, juce::Justification::left);
    g.setColour(gold);
    g.setFont(juce::Font(juce::FontOptions(14.0f, juce::Font::bold)));
    g.drawText("B A S S   /   T U B E  P R E A M P", 55, 72, 360, 22, juce::Justification::left);
    g.setColour(muted);
    g.setFont(juce::Font(juce::FontOptions(10.0f, juce::Font::bold)));
    g.drawText("YMY SOUNDS  •  DOMINICAN LOW-END LAB", 55, 94, 360, 18, juce::Justification::left);

    // Status lamps and model badge.
    g.setColour(panel);
    g.fillRoundedRectangle(w - 310, 38, 230, 63, 8.0f);
    g.setColour(juce::Colour(0xFF51595B));
    g.drawRoundedRectangle(w - 310, 38, 230, 63, 8.0f, 1.0f);
    g.setColour(juce::Colour(0xFF57D68D));
    g.fillEllipse(w - 291, 52, 10, 10);
    g.setColour(cream);
    g.setFont(juce::Font(juce::FontOptions(11.0f, juce::Font::bold)));
    g.drawText("ENGINE ONLINE", w - 275, 49, 155, 17, juce::Justification::left);
    g.setColour(gold);
    g.setFont(juce::Font(juce::FontOptions(9.0f, juce::Font::bold)));
    g.drawText("VST3  •  STEREO  •  48 kHz READY", w - 291, 72, 195, 16, juce::Justification::left);

    // Main control deck.
    const float deckY = 132.0f;
    const float deckH = h - 238.0f;
    g.setColour(juce::Colour(0xFF090B0C));
    g.fillRoundedRectangle(38.0f, deckY + 4.0f, w - 76.0f, deckH, 12.0f);
    juce::ColourGradient deckGradient(juce::Colour(0xFF303638), 0, deckY,
                                      juce::Colour(0xFF1A1E20), 0, deckY + deckH, false);
    g.setGradientFill(deckGradient);
    g.fillRoundedRectangle(38.0f, deckY, w - 76.0f, deckH, 12.0f);
    g.setColour(juce::Colour(0xFF5B6264));
    g.drawRoundedRectangle(38.0f, deckY, w - 76.0f, deckH, 12.0f, 1.2f);

    g.setColour(gold);
    g.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::bold)));
    g.drawText("SIGNAL CHAIN", 58, 147, 180, 18, juce::Justification::left);
    g.setColour(muted);
    g.setFont(juce::Font(juce::FontOptions(10.0f)));
    g.drawText("INPUT  →  PREAMP  →  DRIVE  →  OUTPUT", 58, 166, 360, 17, juce::Justification::left);
    g.setColour(juce::Colour(0xFF60686A));
    g.drawLine(58, 190, w - 58, 190, 1.0f);

    // Individual engraved control bays behind the attached real knobs.
    const float left = 55.0f;
    const float right = w - 55.0f;
    const float gap = 12.0f;
    const float bayW = (right - left - gap * 5.0f) / 6.0f;
    for (int i = 0; i < 6; ++i)
    {
        const float x = left + i * (bayW + gap);
        g.setColour(juce::Colour(0xFF191D1F));
        g.fillRoundedRectangle(x, 207.0f, bayW, 260.0f, 8.0f);
        g.setColour(juce::Colour(0xFF4D5557));
        g.drawRoundedRectangle(x, 207.0f, bayW, 260.0f, 8.0f, 1.0f);
        g.setColour(gold.withAlpha(0.65f));
        g.fillRoundedRectangle(x + 12.0f, 214.0f, bayW - 24.0f, 2.0f, 1.0f);
    }

    // Lower instrument-style VU window, decorative scale.
    const float meterX = 58.0f;
    const float meterY = h - 155.0f;
    const float meterW = 300.0f;
    const float meterH = 70.0f;
    g.setColour(juce::Colour(0xFF0B0D0E));
    g.fillRoundedRectangle(meterX, meterY, meterW, meterH, 7.0f);
    g.setColour(juce::Colour(0xFF596164));
    g.drawRoundedRectangle(meterX, meterY, meterW, meterH, 7.0f, 1.0f);
    g.setColour(juce::Colour(0xFFE2D2AF));
    g.setFont(juce::Font(juce::FontOptions(10.0f, juce::Font::bold)));
    g.drawText("VU / LEVEL", meterX + 12, meterY + 7, 90, 14, juce::Justification::left);
    for (int i = 0; i < 13; ++i)
    {
        const float x = meterX + 18.0f + i * 21.0f;
        g.setColour(i > 9 ? juce::Colour(0xFFD86D4D) : (i > 7 ? gold : juce::Colour(0xFF8DAE8B)));
        g.fillRoundedRectangle(x, meterY + 34.0f, 13.0f, 13.0f, 2.0f);
    }

    // Output path and branding plate.
    const float plateX = w - 365.0f;
    g.setColour(panel);
    g.fillRoundedRectangle(plateX, meterY, 300.0f, meterH, 7.0f);
    g.setColour(juce::Colour(0xFF596164));
    g.drawRoundedRectangle(plateX, meterY, 300.0f, meterH, 7.0f, 1.0f);
    g.setColour(gold);
    g.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::bold)));
    g.drawText("BUILT FOR BACHATA", plateX + 12, meterY + 12, 270, 18, juce::Justification::centred);
    g.setColour(cream);
    g.setFont(juce::Font(juce::FontOptions(10.0f)));
    g.drawText("Warm body  •  Tight attack  •  Solid low end", plateX + 10, meterY + 36, 280, 16, juce::Justification::centred);

    g.setColour(gold);
    g.setFont(juce::Font(juce::FontOptions(10.0f, juce::Font::bold)));
    g.drawText("ORYMY BASS  /  ANALOG-INSPIRED PROCESSING", 58, h - 62, w - 116, 16, juce::Justification::centredLeft);
}
 
void OrymyBassAudioProcessorEditor::resized()
{
    const int margin = 55;
    presetLabel.setBounds(margin, 196, 62, 25);
    presetBox.setBounds(margin + 64, 194, juce::jmin(320, getWidth() / 3), 28);

    const int usableWidth = getWidth() - margin * 2;
    const int columnWidth = usableWidth / static_cast<int>(controls.size());
    const int knobSize = juce::jmin(124, columnWidth - 12);
    const int knobY = 264;

    for (size_t i = 0; i < controls.size(); ++i)
    {
        const int x = margin + static_cast<int>(i) * columnWidth + (columnWidth - knobSize) / 2;
        controlLabels[i].setBounds(margin + static_cast<int>(i) * columnWidth, 225, columnWidth, 24);
        controls[i].setBounds(x, knobY, knobSize, knobSize + 32);
    }
}

void OrymyBassAudioProcessorEditor::timerCallback()
{
    // Keep audio processing on the audio thread; UI metering can be added separately.
}
