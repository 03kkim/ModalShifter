/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HelloSamplerAudioProcessorEditor::HelloSamplerAudioProcessorEditor (HelloSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mLoadButton.onClick = [&]() { audioProcessor.loadFile(); };
    addChildComponent(mLoadButton);

    
    setSize (600, 200);
    
    audioProcessor.thumbnail.addChangeListener(this);            // [6]
}

HelloSamplerAudioProcessorEditor::~HelloSamplerAudioProcessorEditor()
{
}

//==============================================================================
void HelloSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Colours::black);
    
    // if new file dropped
        // get waveform from processor
        
    
    g.setColour(Colours::white);
    g.setFont(15.0f);
    juce::Rectangle<int> thumbnailBounds (10, 100, getWidth() - 20, getHeight() - 120);
     
    if (audioProcessor.thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded (g, thumbnailBounds);
    else
        paintIfFileLoaded (g, thumbnailBounds);
//    if (audioProcessor.getNumSamplerSounds() > 0) {
//        g.fillAll(Colours::red);
//        g.drawText("Sound Loaded", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, Justification::centred);
//        
//    }
//    else {
//        g.drawText("Load a Sound", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, Justification::centred);
//    }
}
// Refactor at some point lol
void HelloSamplerAudioProcessorEditor::paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour (juce::Colours::darkgrey);
    g.fillRect (thumbnailBounds);
    g.setColour (juce::Colours::white);
    mLoadButton.setVisible(true);
    g.drawFittedText ("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
    
}
void HelloSamplerAudioProcessorEditor::paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
{
    g.setColour (juce::Colours::white);
    g.fillRect (thumbnailBounds);

    g.setColour (juce::Colours::red);                               // [8]

    audioProcessor.thumbnail.drawChannels (g,                                      // [9]
                            thumbnailBounds,
                            0.0,                                    // start time
                            audioProcessor.thumbnail.getTotalLength(),             // end time
                            1.0f);                                  // vertical zoom
    mLoadButton.setVisible(false);
}

void HelloSamplerAudioProcessorEditor::resized()
{
    mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
}

bool HelloSamplerAudioProcessorEditor::isInterestedInFileDrag(const StringArray &files)
{
    for (String file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif") ) {
            return true;
        }
    }
    return false;
}

void HelloSamplerAudioProcessorEditor::filesDropped(const StringArray &files, int x, int y)
{
    for (String file : files)
    {
        if (isInterestedInFileDrag(files))
        {
            audioProcessor.loadFile(file);
        }
    }
    
    repaint();
}

