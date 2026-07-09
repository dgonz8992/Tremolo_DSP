//
// Created by David Gonzalez on 7/8/26.
//

#include "Square.h"

void Square::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    const juce::ColourGradient gradientBox
    (
        juce::Colours::lightblue, bounds.getHeight(), bounds.getX(),
        juce::Colours::deeppink, bounds.getRight(), bounds.getBottom(),
        false
    );

    const juce::ColourGradient gradientText
(
    juce::Colours::darkblue, bounds.getX(), bounds.getY(),
    juce::Colours::cyan, bounds.getRight(), bounds.getBottom(),
    false
);

    g.setGradientFill(gradientBox);
    g.fillEllipse(bounds);

    g.setGradientFill (gradientText);
    g.setFont(17.0f);
    g.drawFittedText("Talkin' Tremolo\n", getLocalBounds(), juce::Justification::centred, 2);

    g.setGradientFill(gradientText);
    g.setFont(12.0f);
    g.setOpacity(0.5f);
    const auto textBounds = getLocalBounds().withTrimmedBottom(20);
    g.drawFittedText("Parametrics", textBounds, juce::Justification::centredBottom, 2);
}
