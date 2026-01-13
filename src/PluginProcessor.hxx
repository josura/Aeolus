#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include "DSP/SampleManager.hxx"
#include "DSP/Envelopes.hxx"
#include "DSP/GrainEngine.hxx"

class PruvulazzuAudioProcessor : public juce::AudioProcessor
{
public:
    PruvulazzuAudioProcessor();
    ~PruvulazzuAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Pruvulazzu"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock&) override {}
    void setStateInformation (const void*, int) override {}

    void triggerTestNote() { noteTriggered = true; }

    std::vector<float> getActiveGrainPositions() const;

    // Helpers for the UI
    SampleManager& getSampleManager() { return sampleManager; }
    juce::AudioFormatManager& getFormatManager() { return formatManager; }

private:
    SampleManager sampleManager;
    juce::AudioFormatManager formatManager;
    int playhead = 0;
    bool isNoteActive = false; // MIDI triggering flag
    std::atomic<bool> noteTriggered { false }; // For test note triggering
    std::unique_ptr<Envelope> activeEnvelope = std::make_unique<ParabolicEnvelope>(); // Example envelope
    GrainEngine grainEngine;
    std::atomic<float> currentPlayheadPos { -1.0f }; // 0.0 to 1.0 for UI

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PruvulazzuAudioProcessor)
};