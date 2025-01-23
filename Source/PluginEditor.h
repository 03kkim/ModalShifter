/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class HelloSamplerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                          public FileDragAndDropTarget,
                                          private ChangeListener
{
public:
    HelloSamplerAudioProcessorEditor (HelloSamplerAudioProcessor&);
    ~HelloSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool isInterestedInFileDrag(const StringArray &files) override;
    void filesDropped(const StringArray &files, int x, int y) override;
    
    void paintIfNoFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    void paintIfFileLoaded (juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds);
    
    



private:
    TextButton mLoadButton { "Load" };
    
    HelloSamplerAudioProcessor& audioProcessor;
    
    
    
    void thumbnailChanged() { repaint(); }
    void changeListenerCallback (juce::ChangeBroadcaster* source) override
    {
//        if (source == &transportSource) transportSourceChanged();
        if (source == &audioProcessor.thumbnail)       thumbnailChanged();
    }
    


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelloSamplerAudioProcessorEditor)
};
