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
    addAndMakeVisible(mLoadButton);
    
    setSize (600, 200);
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
    if (audioProcessor.getNumSamplerSounds() > 0) {
        g.fillAll(Colours::red);
        g.drawText("Sound Loaded", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, Justification::centred);
        
    }
    else {
        g.drawText("Load a Sound", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, Justification::centred);
    }
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
