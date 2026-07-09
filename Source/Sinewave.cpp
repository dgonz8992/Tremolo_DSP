//
// Created by David Gonzalez on 6/30/26.
//

#include "Sinewave.h"

void Sinewave::prepare(double sampleRate, const int numChannels)
{
    currentSampleRate = static_cast<float>(sampleRate);
    phases.resize(static_cast<size_t>(numChannels));
    smoothAmp.reset(sampleRate, 0.2f);
    smoothAmp.setCurrentAndTargetValue(getAmplitude());
    smoothFreq.reset(sampleRate, 0.2f);
    smoothFreq.setCurrentAndTargetValue(getFrequency());

}

void Sinewave::process(juce::AudioBuffer<float>& buffer)
{
    if (phases.size() != static_cast<size_t>(buffer.getNumChannels()))
        return;

    for (int channel = 0; channel < buffer.getNumChannels(); ++ channel)
    {
        auto* output = buffer.getWritePointer(channel);
        auto& phase = phases[static_cast<size_t>(channel)];

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const float frequency = smoothFreq.getNextValue();
            const float amplitude = smoothAmp.getNextValue();
            const float phaseInc = (doublePi * frequency) / currentSampleRate;

            output[sample] += amplitude * std::sinf(phase);
            output[sample] += amplitude * (phase / juce::MathConstants<float>::pi) - 1.0f;
            output[sample] += (phase < juce::MathConstants<float>::pi) ? amplitude : -amplitude;

            phase += phaseInc;
            if (phase >= doublePi)
            {
                phase -= doublePi;
            }
        }

    }

}
