/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "KnobLookAndFeel.h"

//==============================================================================
/**
*/
class ChippyAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ChippyAudioProcessorEditor (ChippyAudioProcessor&);
    ~ChippyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:


  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  ChippyAudioProcessor& audioProcessor;

  juce::Slider dsSlider;
  juce::Slider bdSlider;

  KnobLookAndFeel knobLaf;

  float zoomFactor = 1.0f;
  int scrollOffset = 0;

  juce::Colour chippydark;

  juce::Font malekFont;

  juce::Label downsampleLabel;
  juce::Label bitdepthLabel;
  juce::Label titleLabel;

  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>  dsSliderAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>  bdSliderAttachment;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChippyAudioProcessorEditor)
};
