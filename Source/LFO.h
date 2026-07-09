//
// Created by David Gonzalez on 6/30/26.
//

#ifndef LFO_LFO_H
#define LFO_LFO_H

#include <juce_audio_processors/juce_audio_processors.h>

class LFO
{

public:
    void prepare(double sampleRate, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);

    [[nodiscard]] float getAmplitude() const {return smoothAmp.getCurrentValue();}
    [[nodiscard]] float getFrequency() const {return smoothFreq.getCurrentValue();}
    [[nodiscard]] int getShape () const {return shape;}
    void setAmplitude(const float newAmplitude) {smoothAmp.setTargetValue(newAmplitude);}
    void setFrequency(const float newFrequency) {smoothFreq.setTargetValue(newFrequency);}
    void setShape(const int newShape) {shape = newShape;}
    void setSync(const int knobPosition, const float beatsPerSecond, const double ppqPosition);


private:

    float currentSampleRate = 0.0f;
    std::vector<float> phases;

    juce::SmoothedValue<float> smoothAmp;
    juce::SmoothedValue<float> smoothFreq;
    int shape = 0;
    int sync = 2;

    std::vector<float> filterMemories;
    float filterAlpha = 0.007f;

    static constexpr float doublePi = juce::MathConstants<float>::twoPi;
};


#endif //LFO_LFO_H