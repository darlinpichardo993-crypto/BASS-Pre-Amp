#include "PluginProcessor.h"
#include "PluginEditor.h"

OrymyBassAudioProcessorEditor::OrymyBassAudioProcessorEditor(OrymyBassAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
    
    setSize(1100, 720);
    startTimerHz(30);

    addAndMakeVisible(presetBox);
    presetBox.addItem("Bachata Antony Santos", 1);
    presetBox.addItem("Raulin Rodriguez", 2);
    presetBox.addItem("Romeo Santos", 3);
    presetBox.addItem("Max Santos Bass Style", 4);
    presetBox.addItem("Slap Bachata", 5);
    presetBox.setSelectedId(1, juce::dontSendNotification);
}

OrymyBassAudioProcessorEditor::~OrymyBassAudioProcessorEditor() {
    stopTimer();
}

void OrymyBassAudioProcessorEditor::paint(juce::Graphics& g) {
    // Fondo general gris oscuro elegante con textura simulada
    g.fillAll(juce::Colour(0xFF1E1E1E));

    // Cabecera superior corporativa
    g.setColour(juce::Colour(0xFF2A2A2A));
    g.fillRect(0, 0, getWidth(), 60);

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("O R Y M Y  BASS", 20, 15, 200, 30, juce::Justification::left);

    g.setFont(10.0f);
    g.setColour(juce::Colours::lightgrey);
    g.drawText("Y M Y   S O U N D S   V S T", 20, 35, 200, 20, juce::Justification::left);
    g.drawText("EL SONIDO DE LA BACHATA Y EL MERENGUE", getWidth() - 240, 20, 220, 20, juce::Justification::right);
}

void OrymyBassAudioProcessorEditor::resized() {
    presetBox.setBounds(20, 90, 180, 30);
}

void OrymyBassAudioProcessorEditor::timerCallback() {
    // Actualizaciones periódicas de medidores y UI en tiempo real
}
