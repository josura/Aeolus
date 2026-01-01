#pragma once
#include <JuceHeader.h>
#include "FragmentAudioProcessor.h"
#include "../DSP/FileLoader.hxx"
#include "../UI/WaveformComponent.hxx"

class FragmentAudioProcessorEditor : public juce::AudioProcessorEditor,
                                     public juce::FileDragAndDropTarget
{
public:
    // Constructor
    FragmentAudioProcessorEditor(FragmentAudioProcessor& p)
        : juce::AudioProcessorEditor(&p), processor(p),
          waveformVisualizer(processor.getFormatManager())
    {
        setSize(600, 400);
        addAndMakeVisible(waveformVisualizer);
    }

    bool isInterestedInFileDrag(const juce::StringArray& files) override { return true; }
    
    void filesDropped(const juce::StringArray& files, int, int) override {
        juce::File file(files[0]);
        
        // 1. Update the UI Visualizer
        waveformVisualizer.setFile(file);
        
        // 2. Load the data and push it to the Processor (Audio Thread)
        auto newBuffer = loader.loadFile(file);
        processor.getSampleManager().updateCurrentBuffer(newBuffer);
    }

private:
    WaveformComponent waveformVisualizer;
    FileLoader loader;
    FragmentAudioProcessor& processor;
};