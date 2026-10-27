#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

OrymyBassAudioProcessor::OrymyBassAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout()) {}

OrymyBassAudioProcessor::~OrymyBassAudioProcessor() = default;

const juce::String OrymyBassAudioProcessor::getName() const { return "ORYMY Bass"; }
bool OrymyBassAudioProcessor::acceptsMidi() const { return false; }
bool OrymyBassAudioProcessor::producesMidi() const { return false; }
bool OrymyBassAudioProcessor::isMidiEffect() const { return false; }
double OrymyBassAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int OrymyBassAudioProcessor::getNumPrograms() { return 1; }
int OrymyBassAudioProcessor::getCurrentProgram() { return 0; }
void OrymyBassAudioProcessor::setCurrentProgram(int) {}
const juce::String OrymyBassAudioProcessor::getProgramName(int) { return {}; }
void OrymyBassAudioProcessor::changeProgramName(int, const juce::String&) {}

juce::AudioProcessorValueTreeState::ParameterLayout OrymyBassAudioProcessor::createParameterLayout()
{
    using FloatParam = juce::AudioParameterFloat;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<FloatParam>("input_gain", "Input Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f, "dB"));
    params.push_back(std::make_unique<FloatParam>("output_gain", "Output Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f, "dB"));
    params.push_back(std::make_unique<FloatParam>("amp_gain", "Amp Gain",
        juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), 5.0f));
    params.push_back(std::make_unique<FloatParam>("amp_master", "Amp Master",
        juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), 7.0f));
    params.push_back(std::make_unique<FloatParam>("drive", "Drive",
        juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), 1.0f));
    params.push_back(std::make_unique<FloatParam>("mix", "Saturation Mix",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f, "%"));

    return { params.begin(), params.end() };
}

void OrymyBassAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(juce::jmax(1, samplesPerBlock));
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());

    inputGain.prepare(spec);
    outputGain.prepare(spec);
    inputGain.setRampDurationSeconds(0.02);
    outputGain.setRampDurationSeconds(0.02);
    inputGain.reset();
    outputGain.reset();
}

void OrymyBassAudioProcessor::releaseResources()
{
    inputGain.reset();
    outputGain.reset();
}

bool OrymyBassAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto output = layouts.getMainOutputChannelSet();
    const auto input = layouts.getMainInputChannelSet();
    if (output != juce::AudioChannelSet::mono() && output != juce::AudioChannelSet::stereo())
        return false;
    return input == output;
}

void OrymyBassAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int inputChannels = getTotalNumInputChannels();
    const int outputChannels = getTotalNumOutputChannels();

    for (int channel = inputChannels; channel < outputChannels; ++channel)
        buffer.clear(channel, 0, buffer.getNumSamples());

    const auto readParam = [this](const char* id, float fallback)
    {
        if (auto* value = parameters.getRawParameterValue(id))
            return value->load(std::memory_order_relaxed);
        return fallback;
    };

    inputGain.setGainDecibels(readParam("input_gain", 0.0f));
    outputGain.setGainDecibels(readParam("output_gain", 0.0f));
    auto block = juce::dsp::AudioBlock<float>(buffer);
    inputGain.process(juce::dsp::ProcessContextReplacing<float>(block));

    const float ampGain = readParam("amp_gain", 5.0f);
    const float master = readParam("amp_master", 7.0f);
    const float drive = 1.0f + ampGain * 0.45f + readParam("drive", 1.0f) * 0.65f;
    const float normalizer = std::tanh(drive);
    const float mix = juce::jlimit(0.0f, 1.0f, readParam("mix", 100.0f) * 0.01f);
    const float masterGain = juce::Decibels::decibelsToGain((master - 5.0f) * 1.5f);

    for (int channel = 0; channel < inputChannels; ++channel)
    {
        auto* samples = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const float dry = samples[sample];
            const float saturated = normalizer > 0.0f ? std::tanh(dry * drive) / normalizer : dry;
            samples[sample] = ((dry * (1.0f - mix)) + (saturated * mix)) * masterGain;
        }
    }

    outputGain.process(juce::dsp::ProcessContextReplacing<float>(block));
}

bool OrymyBassAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* OrymyBassAudioProcessor::createEditor()
{
    return new OrymyBassAudioProcessorEditor(*this);
}

void OrymyBassAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    if (auto xml = state.createXml())
        copyXmlToBinary(*xml, destData);
}

void OrymyBassAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xmlState = getXmlFromBinary(data, sizeInBytes))
    {
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OrymyBassAudioProcessor();
}
