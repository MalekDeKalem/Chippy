#include "KnobLookAndFeel.h"

KnobLookAndFeel::KnobLookAndFeel()
{
}

KnobLookAndFeel::~KnobLookAndFeel()
{
}

void KnobLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &)
{
  auto radius = (float) juce::jmin (width / 2, height / 2) - (width / 25.0f / 2.0f);
  auto centreX = (float) x + (float) width  * 0.5f;
  auto centreY = (float) y + (float) height * 0.5f;
  auto rx = centreX - radius;
  auto ry = centreY - radius;
  auto rw = radius * 2.0f;
  auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

  g.setColour(juce::Colours::darkgrey);
  g.fillEllipse(rx, ry, rw, rw);

  juce::Path arcPath;
  arcPath.addCentredArc(centreX, centreY, radius + (width / 25.0f / 2.0f - 5.0f), radius + (width / 25.0f / 2.0f - 5.0f), 0.0f, rotaryStartAngle, rotaryEndAngle, true);
  
  g.setColour(juce::Colours::dimgrey);
  g.strokePath(arcPath, juce::PathStrokeType(width / 25.0f));

  juce::Path pointerArc;
  pointerArc.addCentredArc(centreX, centreY, radius + (width / 25.0f / 2.0f - 5.0f), radius + (width / 25.0f / 2.0f - 5.0f), 0.0f, rotaryStartAngle, angle, true);

  g.setColour(juce::Colour(27, 250, 76));
  g.strokePath(pointerArc, juce::PathStrokeType(width / 25.0f));

  juce::Path p;
  auto pointerLength = radius / 1.5f;
  auto pointerThickness = width / 15.0f / 2.0f;
  auto corner = radius / 100.0f * pointerThickness;

  p.addRoundedRectangle(-pointerThickness * 0.5f, -radius + 5.0f, pointerThickness, pointerLength, corner);
  p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

  g.setColour(juce::Colour(217, 217, 217));
  g.fillPath(p);

}
