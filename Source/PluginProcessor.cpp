#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <atomic>

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), state(*this, nullptr, "parameters", createParameters())
{
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
= default;

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;

}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    juce::ignoreUnused (sampleRate, samplesPerBlock);

    sinewave.prepare(sampleRate, getTotalNumOutputChannels());
    sinewave.setAmplitude(.01f);
    sinewave.setFrequency(220.0f);

    sinewave1.prepare(sampleRate, getTotalNumOutputChannels());
    sinewave1.setAmplitude(.01f);
    sinewave1.setFrequency(277.18f);

    sinewave2.prepare(sampleRate, getTotalNumOutputChannels());
    sinewave2.setAmplitude(.01f);
    sinewave2.setFrequency(329.63f);

    sinewave3.prepare(sampleRate, getTotalNumOutputChannels());
    sinewave3.setAmplitude(.01f);
    sinewave3.setFrequency(415.30f);
    //
    // sinewave4.prepare(sampleRate, getTotalNumOutputChannels());
    // sinewave4.setAmplitude(.1f);
    // sinewave4.setFrequency(493.88f);

    lfo.prepare(sampleRate, getTotalNumOutputChannels());

    lfoDepthParam = state.getRawParameterValue("lfoDepth");
    lfoRateParam = state.getRawParameterValue("lfoRate");
    lfoShapeParam = state.getRawParameterValue("lfoShape");
    lfoSyncParam = state.getRawParameterValue("lfoSync");
    lfoNoteParam = state.getRawParameterValue("lfoNote");


}

void AudioPluginAudioProcessor::releaseResources()
{

}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;

    float currentBpm = 120.0f;
    double currentPpq = 0.0;

    if (const auto* playHead = getPlayHead())
    {
        if (auto positionInfo = playHead->getPosition())
        {
            if (positionInfo->getBpm().hasValue())
                currentBpm = static_cast<float>(*positionInfo->getBpm());

            if (positionInfo->getPpqPosition().hasValue())
            currentPpq = *positionInfo->getPpqPosition();
        }
    }

    const float beatsPerSecond = currentBpm / 60.0f;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        juce::ignoreUnused (channelData);

    }

    const float depth = lfoDepthParam->load();
    const float rate = lfoRateParam->load();
    const int note = static_cast<int>(lfoNoteParam->load());
    const int shape = static_cast<int>(lfoShapeParam->load());
    const bool sync = static_cast<int>(lfoSyncParam->load());

    if (sync == 0) lfo.setFrequency(rate);
    else lfo.setSync(note, beatsPerSecond, currentPpq);

    lfo.setAmplitude(depth);
    lfo.setShape (shape);

    // sinewave.process(buffer);
    // sinewave1.process(buffer);
    // sinewave2.process(buffer);
    // sinewave3.process(buffer);
    // sinewave4.process(buffer);
    // sinewave5.process(buffer);
    // sinewave6.process(buffer);
    lfo.process(buffer);
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto stateXml = state.copyState().createXml();
    copyXmlToBinary(*stateXml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto xmlState = getXmlFromBinary(data, sizeInBytes);
    if (xmlState != nullptr)
        state.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameters()
{
    juce::StringArray noteChoices {"1/1", "1/1d", "1/1t", "1/2", "1/2d", "1/2t",
                                    "1/4", "1/4d", "1/4t", "1/8", "1/8d", "1/8t",
                                    "1/16", "1/16d", "1/16t", "1/32", "1/32d", "1/32t"};

    juce::StringArray shapeChoices {"Sine", "Saw", "Rev Saw", "Square"};

    return
 {
        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"lfoDepth"},
                                                     "LFO Depth",
                                                     0.0f,
                                                     1.0f,
                                                     1.0f),

        std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"lfoRate"},
                                                    "LFO Rate",
                                                    0.0f,
                                                    20.0f,
                                                    5.0f),

        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"lfoNote"},
                                            "LFO Note",
                                            noteChoices,
                                            6),

        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"lfoShape"},
                                                    "LFO Shape",
                                                    shapeChoices,
                                                    0),

        std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"lfoSync"},
                                            "LFO Sync",
                                            false),
    };

}
