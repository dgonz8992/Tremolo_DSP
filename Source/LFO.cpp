//
// Created by David Gonzalez on 6/30/26.
//

#include "LFO.h"


void LFO::prepare(const double sampleRate, const int numChannels)
{
    currentSampleRate = static_cast<float>(sampleRate);
    phases.resize(static_cast<size_t>(numChannels));
    smoothAmp.reset(sampleRate, 0.2f);
    smoothAmp.setCurrentAndTargetValue(getAmplitude());
    smoothFreq.reset(sampleRate, 0.2f);
    smoothFreq.setCurrentAndTargetValue(getFrequency());

    filterMemories.resize(static_cast<size_t>(numChannels), 0.0f);
}

void LFO::setSync(const int knobPosition, const float beatsPerSecond, const double ppqPosition)
{
    const int baseLength = knobPosition / 3;
    const int modifier = knobPosition % 3;

    float multiplier;
    switch (baseLength)
    {
        case 0: multiplier = 0.25f; break;
        case 1: multiplier = 0.5f; break;
        case 2: multiplier = 1.0f; break;
        case 3: multiplier = 2.0f; break;
        case 4: multiplier = 4.0f; break;
        case 5: multiplier = 8.0f; break;
        default: multiplier = 1.0f; break;
    }

    if (modifier == 1) multiplier *= 0.6667f;
    else if (modifier == 2) multiplier *= 1.5000f;

    setFrequency(beatsPerSecond * multiplier);

    double cyclePosition = std::fmod(ppqPosition * static_cast<double>(multiplier), 1.0);
    if (cyclePosition < 0.0) cyclePosition += 1.0;

    const float newPhase = static_cast<float>(cyclePosition) * doublePi;
    for (auto& phase : phases)
        phase = newPhase;

}


void LFO::process(juce::AudioBuffer<float>& buffer)
{

    if (phases.size() != static_cast<size_t>(buffer.getNumChannels()))
        return;

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        const float frequency = smoothFreq.getNextValue();
        const float amplitude = smoothAmp.getNextValue();

        if (frequency == 0.0f) break;

        for (int channel = 0; channel < buffer.getNumChannels(); ++ channel)
        {
            auto* output = buffer.getWritePointer(channel);
            auto& phase = phases[static_cast<size_t>(channel)];
            const float phaseInc = (doublePi * frequency) / currentSampleRate;

            auto& lastFilteredValue = filterMemories[static_cast<size_t>(channel)];

            float normalizedWave = 0;

            switch (getShape())
            {
                case 0:
                {
                    normalizedWave = (std::sinf(phase) + 1.0f) * 0.5f;
                    break;
                }
                case 1:
                {
                    normalizedWave = (((phase / juce::MathConstants<float>::pi) - 1.0f) + 1) * 0.5f;
                    break;
                }
                case 2:
                {
                    normalizedWave = ((1.0f - (phase / juce::MathConstants<float>::pi)) + 1) * 0.5f;
                    break;
                }
                case 3:
                {
                    normalizedWave = (((phase < juce::MathConstants<float>::pi) ? 1.0f : -1.0f) + 1) * 0.5f;
                    break;
                }
                default:
                {
                    normalizedWave = (std::sinf(phase) + 1.0f) * 0.5f;
                    break;
                }
            }

            lastFilteredValue = lastFilteredValue + filterAlpha * (normalizedWave - lastFilteredValue);
            const float modulatedMultiplier = (1 - amplitude) + (amplitude * lastFilteredValue);
            output[sample] *= modulatedMultiplier;

            phase += phaseInc;
            if (phase >= doublePi) phase -= doublePi;

        }

    }

}
