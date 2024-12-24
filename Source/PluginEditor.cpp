/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

//==============================================================================
ChippyAudioProcessorEditor::ChippyAudioProcessorEditor (ChippyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{


  dsSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
  dsSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 25, 10);
  dsSlider.setPopupDisplayEnabled(true, false, this);

  bdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
  bdSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 25, 10);
  bdSlider.setPopupDisplayEnabled(true, false, this);

  dsSlider.setRange(1.0, 100.0, 1.0);
  dsSlider.setValue(1.0);

  bdSlider.setRange(2.0, 32.0, 1.0);
  bdSlider.setValue(32.0);


  dsSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.params, "ds_factor", dsSlider);
  bdSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.params, "bit_depth", bdSlider);

  dsSlider.setLookAndFeel(&knobLaf);
  bdSlider.setLookAndFeel(&knobLaf);

  audioProcessor.waveDisplay.setBufferSize(128);
  audioProcessor.waveDisplay.setRepaintRate(30);
  audioProcessor.waveDisplay.setColours(juce::Colours::black, juce::Colours::whitesmoke.withAlpha(0.5f));

  // define colors
  chippydark = juce::Colour(12, 12, 12);

  auto fontData = juce::MemoryBlock(BinaryData::MalekType_otf, BinaryData::MalekType_otfSize);
  malekFont = juce::Typeface::createSystemTypefaceFor(fontData.getData(), fontData.getSize());

  // make labels
  downsampleLabel.setFont(juce::Font(malekFont).withHeight(15.f));
  downsampleLabel.setText("Downsample", juce::dontSendNotification);
  downsampleLabel.setColour(juce::Label::textColourId, juce::Colour(27, 250, 76));
  downsampleLabel.setJustificationType(juce::Justification::centred);

  bitdepthLabel.setFont(juce::Font(malekFont).withHeight(15.f));
  bitdepthLabel.setText("Bitdepth", juce::dontSendNotification);
  bitdepthLabel.setColour(juce::Label::textColourId, juce::Colour(27, 250, 76));
  bitdepthLabel.setJustificationType(juce::Justification::centred);

  titleLabel.setFont(juce::Font(malekFont).withHeight(20.f));
  titleLabel.setText("CHIPPY", juce::dontSendNotification);
  titleLabel.setColour(juce::Label::textColourId, juce::Colour(27, 250, 76));
  titleLabel.setJustificationType(juce::Justification::centred);

  // Add Components
  addAndMakeVisible(audioProcessor.waveDisplay);
  addAndMakeVisible(dsSlider);
  addAndMakeVisible(bdSlider);
  addAndMakeVisible(downsampleLabel);
  addAndMakeVisible(bitdepthLabel);
  addAndMakeVisible(titleLabel);


  // setResizable(true, true);
  // setResizeLimits(240, 180, 600, 450);
  // 400 300 original size
  setSize(480, 360);
}

ChippyAudioProcessorEditor::~ChippyAudioProcessorEditor()
{
  audioProcessor.waveDisplay.clear();
  dsSlider.setLookAndFeel(nullptr);
  bdSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void ChippyAudioProcessorEditor::paint (juce::Graphics& g)
{
  g.fillAll(chippydark);
}

void ChippyAudioProcessorEditor::resized()
{
  int knobSize = getWidth() / 4;
  int waveDisplayHeight = getHeight() / 3;
  int waveDisplayPadding = 20;
  int waveDisplayWidth = getWidth() - 2 * waveDisplayPadding;
  int dsSliderPosX = (getWidth() / 2) - (knobSize + knobSize / 2);
  int dsSliderPosY = waveDisplayPadding + waveDisplayHeight + (getHeight() - (waveDisplayPadding + waveDisplayHeight)) / 2 - knobSize / 2;
  int bdSliderPosX = getWidth() - getWidth() / 2 + knobSize / 2;
  int labelHeight = 25;

  audioProcessor.waveDisplay.setBounds(waveDisplayPadding, waveDisplayPadding, waveDisplayWidth, waveDisplayHeight);
  dsSlider.setBounds(dsSliderPosX, dsSliderPosY, knobSize, knobSize);
  bdSlider.setBounds(bdSliderPosX, dsSliderPosY, knobSize, knobSize);
  downsampleLabel.setBounds(((knobSize + dsSliderPosX) - getWidth()/3)/2 + dsSliderPosX/2, getHeight() - 40 ,getWidth() / 3, labelHeight);
  bitdepthLabel.setBounds(((knobSize + bdSliderPosX) - getWidth()/3)/2 + bdSliderPosX/2, getHeight() - 40, getWidth() / 3, labelHeight);
  titleLabel.setBounds((getWidth() - getWidth()/3)/2, waveDisplayHeight + waveDisplayPadding + 20, getWidth() / 3, labelHeight);
}


