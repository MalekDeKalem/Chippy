/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


ChippyAudioProcessor::ChippyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        // params(*this, nullptr, "PARAMETERS",
        //     {
        //         std::make_unique<juce::AudioParameterInt>("ds_factor", "Downsampling", 1, 100, 1),
        //         std::make_unique<juce::AudioParameterInt>("bit_depth", "BitDepth", 2, 32, 32)
        //     }
        // ),
        // waveDisplay(1)
#else
    : 
#endif
    params(*this, nullptr, juce::Identifier("PARAMETERS"), 
        {
            std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"ds_factor", 1}, "Downsampling", 1, 100, 1),
            std::make_unique<juce::AudioParameterInt>(juce::ParameterID{"bit_depth", 1}, "BitDepth", 2, 32, 32)
        }
    ),
    waveDisplay(1)
{
    downSampleParameter = params.getRawParameterValue("ds_factor");
    bitDepthParameter = params.getRawParameterValue("bit_depth");
}

ChippyAudioProcessor::~ChippyAudioProcessor()
{
}


const juce::String ChippyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChippyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChippyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ChippyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ChippyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChippyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ChippyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChippyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ChippyAudioProcessor::getProgramName (int index)
{
    return {};
}

void ChippyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ChippyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ChippyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChippyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ChippyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    int numSamples = buffer.getNumSamples();

    // int downSample = std::max(1, static_cast<int>(*params.getRawParameterValue("ds_factor"))); 
    // int bitDepth = std::max(1, static_cast<int>(*params.getRawParameterValue("bit_depth")));

    int downSample = static_cast<int>(*downSampleParameter); 
    int bitDepth = static_cast<int>(*bitDepthParameter);
    bitDepth = bitDepth == 32 ? 31 : bitDepth;

    float maxAmplitude = 1.0f; 
    int numQuantizationLevels = 1 << bitDepth; 
    float quantizationStep = 2.0f * maxAmplitude / numQuantizationLevels;
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        float prevSample = 0.0f;
        
        for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
        {
            if (sampleIndex % downSample == 0) 
            {
                float sample = channelData[sampleIndex];
                float quantizedSample = std::round(sample / quantizationStep) * quantizationStep;
                prevSample = quantizedSample;
            }

            channelData[sampleIndex] = prevSample;
        }

    }

    waveDisplay.pushBuffer(buffer);

}

//==============================================================================
bool ChippyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ChippyAudioProcessor::createEditor()
{
    return new ChippyAudioProcessorEditor (*this);
}

//==============================================================================
void ChippyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = params.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ChippyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (params.state.getType()))
            params.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChippyAudioProcessor();
}
