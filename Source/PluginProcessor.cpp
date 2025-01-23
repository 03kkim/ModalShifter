/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HelloSamplerAudioProcessor::HelloSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                      thumbnailCache (5),                            // [4]
                      thumbnail (512, mFormatManager, thumbnailCache) // [5]
#endif
{
    mFormatManager.registerBasicFormats();
    for (int i = 0; i < mNumVoices; i++) {
        mSampler.addVoice(new juce::SamplerVoice());
    }
}

HelloSamplerAudioProcessor::~HelloSamplerAudioProcessor()
{
    mFormatReader = nullptr;
}

//==============================================================================
const juce::String HelloSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HelloSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HelloSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HelloSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HelloSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HelloSamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HelloSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HelloSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HelloSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void HelloSamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HelloSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
}

void HelloSamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HelloSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void HelloSamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool HelloSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HelloSamplerAudioProcessor::createEditor()
{
    return new HelloSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void HelloSamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HelloSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

// USING CHOOSER
void HelloSamplerAudioProcessor::loadFile()
{
    mSampler.clearSounds();
    
    FileChooser chooser{"Please load a file"};
    
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor(file);
        thumbnail.setSource (new juce::FileInputSource (file));
    }
    
    BigInteger range;
    range.setRange(0, 128, true);
    
    mSampler.addSound (new SamplerSound("Sample", *mFormatReader, range, 60, 0., 0.1, 10));
}
void HelloSamplerAudioProcessor::loadFile(const String& path)
{
    mSampler.clearSounds();
    
    File file = File(path);
    mFormatReader = mFormatManager.createReaderFor(file);
//    int lenSamples = static_cast<int>(mFormatReader->lengthInSamples);
    
//    for (int sample = 0; sample < mWaveForm.getNumSamples(); sample++)
//    {
//        DBG(buffer[sample]);
//    }
    
    BigInteger range;
    range.setRange(0, 128, true);
    
    mSampler.addSound (new SamplerSound("Sample", *mFormatReader, range, 60, 0., 0.1, 10));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HelloSamplerAudioProcessor();
}
