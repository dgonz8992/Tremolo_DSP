#pragma once

#include "PluginProcessor.h"
#include "Square.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    Square square;
    AudioPluginAudioProcessor& processorRef;

    juce::Slider lfoDepth;
    juce::Label lfoDepthLabel {"lfoDepthLabel", "LFO Depth"};
    juce::Slider lfoRate;
    juce::Label lfoRateLabel {"lfoRateLabel", "LFO Rate"};
    juce::Slider lfoShape;
    juce::Label lfoShapeLabel {"lfoShapeLabel", "LFO Shape"};
    juce::TextButton lfoSync;
    juce::Slider lfoNote;
    juce::Label lfoNoteLabel {"lfoNoteLabel", "LFO Note"};


    juce::AudioProcessorValueTreeState::SliderAttachment lfoDepthAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment lfoRateAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment lfoShapeAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment lfoSyncAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment lfoNoteAttachment;


    juce::Image backgroundImage;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
