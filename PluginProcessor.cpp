#include "PluginProcessor.h"
#include "PluginEditor.h"

OrymyBassAudioProcessor::OrymyBassAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout()) {}

OrymyBassAudioProcessor::~OrymyBassAudioProcessor() {}

const juce::String OrymyBassAudioProcessor::getName() const { return "ORYMY Bass"; }
bool OrymyBassAudioProcessor::acceptsMidi() const { return false; }
bool OrymyBassAudioProcessor::producesMidi() const { return false; }
bool OrymyBassAudioProcessor::isMidiEffect() const { return false; }
double OrymyBassAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int OrymyBassAudioProcessor::getNumPrograms() { return 15; }
int OrymyBassAudioProcessor::getCurrentProgram() { return 0; }
void OrymyBassAudioProcessor::setCurrentProgram(int index) {}
const juce::String OrymyBassAudioProcessor::getProgramName(int index) { return "Preset " + juce::String(index + 1); }
void OrymyBassAudioProcessor::changeProgramName(int index, const juce::String& newName) {}

juce::AudioProcessorValueTreeState::ParameterLayout OrymyBassAudioProcessor::createParameterLayout() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("input_gain", "Input Gain", -24.0f, 24.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("output_gain", "Output Gain", -24.0f, 24.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("amp_gain", "Amp Gain", 0.0f, 10.0f, 5.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("amp_master", "Amp Master", 0.0f, 10.0f, 7.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("drive", "Drive", 0.0f, 10.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("mix", "Cabinet Mix", 0.0f, 100.0f, 100.0f));

    return { params.begin(), params.end() };
}

void OrymyBassAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());

    inputGain.prepare(spec);
    outputGain.prepare(spec);
    
    inputGain.setRampDurationMs(50);
    outputGain.setRampDurationMs(50);
}

void OrymyBassAudioProcessor::releaseResources() {}

bool OrymyBassAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    if (layouts.outputFmt != juce::AudioChannelSet::mono() && layouts.outputFmt != juce::AudioChannelSet::stereo())
        return false;
    if (layouts.outputFmt != layouts.inputFmt)
        return false;
    return true;
}

void OrymyBassAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block(buffer);
    inputGain.process(juce::dsp::ProcessContextReplacing<float>(block));
    outputGain.process(juce::dsp::ProcessContextReplacing<float>(block));
}

bool OrymyBassAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* OrymyBassAudioProcessor::createEditor() { return new OrymyBassAudioProcessorEditor(*this); }

void OrymyBassAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void OrymyBassAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new OrymyBassAudioProcessor();
}
