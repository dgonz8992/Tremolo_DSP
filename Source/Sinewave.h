//
// Created by David Gonzalez on 6/30/26.
//

#ifndef LFO_Sinewave_H
#define LFO_Sinewave_H

#include <juce_audio_processors/juce_audio_processors.h>

class Sinewave
{

public:
    void prepare(double sampleRate, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);

    [[nodiscard]] float getAmplitude() {return smoothAmp.getNextValue();}
    [[nodiscard]] float getFrequency() {return smoothFreq.getNextValue();}
    void setAmplitude(const float newAmplitude) {smoothAmp.setCurrentAndTargetValue(newAmplitude);}
    void setFrequency(const float newFrequency) {smoothFreq.setCurrentAndTargetValue(newFrequency);}


private:

    float currentSampleRate = 0.0f;
    std::vector<float> phases;
    static constexpr float doublePi = juce::MathConstants<float>::twoPi;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> smoothAmp;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> smoothFreq;
};


#endif //LFO_Sinewave_H