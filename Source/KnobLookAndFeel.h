#pragma once

#include <JuceHeader.h>

class KnobLookAndFeel : public juce::LookAndFeel_V4
{
  public:
    KnobLookAndFeel();
    ~KnobLookAndFeel() override;

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
};