#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.hxx"
#include "DSP/FileLoader.hxx"
#include "UI/WaveformComponent.hxx"

class AeolusAudioProcessorEditor : public juce::AudioProcessorEditor,
                                   public juce::FileDragAndDropTarget
{
public:
    AeolusAudioProcessorEditor(AeolusAudioProcessor&);
    ~AeolusAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    bool isInterestedInFileDrag(const juce::StringArray&) override;
    void filesDropped(const juce::StringArray&, int, int) override;

private:
    AeolusAudioProcessor& processor;
    WaveformComponent waveformVisualizer;
    FileLoader loader;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AeolusAudioProcessorEditor)
};