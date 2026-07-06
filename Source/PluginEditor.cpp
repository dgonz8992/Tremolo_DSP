#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <BinaryData.h>



//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), lfoDepthAttachment (processorRef.getState(), "lfoDepth", lfoDepth),
                                                    lfoRateAttachment(processorRef.getState(), "lfoRate", lfoRate),
                                                    lfoShapeAttachment(processorRef.getState(), "lfoShape", lfoShape),
                                                    lfoSyncAttachment(processorRef.getState(), "lfoSync", lfoSync),
                                                    lfoNoteAttachment(processorRef.getState(), "lfoNote", lfoNote)

{
    juce::ignoreUnused (processorRef);
    setSize (400, 400);

    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::GradientBackground_png,
                                                      BinaryData::GradientBackground_pngSize);

    lfoDepth.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    lfoDepth.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 40, 30);
    lfoDepth.setColour(juce::Slider::textBoxTextColourId, juce::Colours::darkgrey);
    lfoDepthLabel.setJustificationType(juce::Justification::centred);

    lfoRate.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    lfoRate.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 30);
    lfoRate.setColour(juce::Slider::textBoxTextColourId, juce::Colours::darkgrey);

    lfoNote.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    lfoNote.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 30);
    lfoNote.setColour(juce::Slider::textBoxTextColourId, juce::Colours::darkgrey);

    lfoRateLabel.setJustificationType(juce::Justification::centred);

    lfoShape.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    lfoShape.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50 , 30);
    lfoShape.setColour(juce::Slider::textBoxTextColourId, juce::Colours::darkgrey);
    lfoShapeLabel.setJustificationType(juce::Justification::centred);

    lfoSync.setButtonText (juce::CharPointer_UTF8 ("\xE2\x99\xAA"));
    lfoSync.setToggleState(false, juce::NotificationType::dontSendNotification);
    lfoSync.setClickingTogglesState(true);
    lfoSync.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lightblue);
    lfoSync.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkblue);
    lfoSync.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::darkgrey);
    lfoSync.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);

    addAndMakeVisible(lfoDepth);
    addAndMakeVisible(lfoShape);
    addAndMakeVisible(lfoDepthLabel);
    addAndMakeVisible(lfoRate);
    addAndMakeVisible(lfoRateLabel);
    addAndMakeVisible(lfoShapeLabel);
    addAndMakeVisible(lfoSync);
    addAndMakeVisible(lfoNote);


    lfoSync.onClick = [this]()
    {
      const bool sync = lfoSync.getToggleState();
        lfoNote.setVisible(sync);
        lfoRate.setVisible(!sync);
    };

    const bool initialSync = lfoSync.getToggleState();
    lfoNote.setVisible(initialSync);
    lfoRate.setVisible(!initialSync);

}


AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
= default;

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);

    if (backgroundImage.isValid())
    {
        g.drawImage (backgroundImage,
                    getLocalBounds().toFloat(),
                    juce::RectanglePlacement::fillDestination);
    }
}

void AudioPluginAudioProcessorEditor::resized()
{

    lfoDepthLabel.setBounds(getWidth() / 2 + 32, getHeight() / 2 + 165, 100, 20);
    lfoDepth.setBounds(getWidth() / 2 + 20, getHeight() / 2 + 55, 125, 115);

    lfoRateLabel.setBounds(getWidth() / 2 + 32, getHeight() / 2 - 180, 100, 20);
    lfoRate.setBounds(getWidth() / 2 + 20, getHeight() / 2 - 160, 125, 115);

    lfoNote.setBounds(getWidth() / 2 + 20, getHeight() / 2 - 160, 125, 115);

    lfoShapeLabel.setBounds(getWidth() / 2 - 148, getHeight() / 2 - 70, 90, 20);
    lfoShape.setBounds(getWidth() / 2 - 160, getHeight() / 2 - 50, 125, 115);

    lfoSync.setBounds(getWidth() / 2 - 15, getHeight() / 2 - 15, 30, 30);

}
