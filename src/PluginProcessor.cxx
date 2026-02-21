/**
 * Pruvulazzu - A JUCE-based granular synthesizer plugin
 * Copyright (C) 2026  Giorgio Locicero
 * @brief This file contains the implementation of the main audio processor class for the Pruvulazzu plugin.
 */
#include "PluginProcessor.hxx"
#include "PluginEditor.hxx"

PruvulazzuAudioProcessor::PruvulazzuAudioProcessor() 
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "Parameters", {
          std::make_unique<juce::AudioParameterFloat> ("density", "Density", 10.0f, 1000.0f, 100.0f),
          std::make_unique<juce::AudioParameterFloat> ("size", "Grain Size", 10.0f, 500.0f, 100.0f)
      })
{
    formatManager.registerBasicFormats();
}

PruvulazzuAudioProcessor::~PruvulazzuAudioProcessor() {}

// void PruvulazzuAudioProcessor::prepareToPlay (double, int) { playhead = 0; }

void PruvulazzuAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    grainEngine.prepare(100); // Allow up to 100 simultaneous grains
    samplesSinceLastGrain = 0;
}

void PruvulazzuAudioProcessor::releaseResources() {}

// void PruvulazzuAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
// {
//     juce::ScopedNoDenormals noDenormals;

//     // Check if a test note has been triggered from the UI
//     if (noteTriggered.exchange(false))
//     {
//         // Inject a MIDI Note On (Note 60, Velocity 1.0) into the buffer
//         midiMessages.addEvent(juce::MidiMessage::noteOn(1, 60, 1.0f), 0);
//     }

//     // Handle MIDI
//     for (const auto metadata : midiMessages) {
//         auto msg = metadata.getMessage();
//         if (msg.isNoteOn()) {
//             playhead = 0;
//             isNoteActive = true;
//         } else if (msg.isNoteOff()) {
//             isNoteActive = false;
//         }
//     }

//     buffer.clear();
//     if (!isNoteActive) return;

//     auto currentData = sampleManager.getCurrentBuffer();
//     if (currentData == nullptr) return;

//     const auto& sampleBuffer = currentData->getBuffer();
//     auto* leftOut = buffer.getWritePointer(0);
//     auto* rightOut = buffer.getWritePointer(1);
//     const auto* sampleIn = sampleBuffer.getReadPointer(0);

//     for (int i = 0; i < buffer.getNumSamples(); ++i) {
//         if (playhead < sampleBuffer.getNumSamples()) {
//             float sample = sampleIn[playhead];
//             leftOut[i] += sample;
//             if (buffer.getNumChannels() > 1) rightOut[i] += sample;
//             playhead++;
//         }
//     }
// }

std::vector<float> PruvulazzuAudioProcessor::getActiveGrainPositions() const  {
    auto currentData = sampleManager.getCurrentBuffer();
    int totalLen = (currentData != nullptr) ? currentData->getBuffer().getNumSamples() : 0;
    return grainEngine.getActivePositions(totalLen);
}

// void PruvulazzuAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
//     juce::ScopedNoDenormals noDenormals;
//     buffer.clear();

//     auto currentData = sampleManager.getCurrentBuffer();
//     if (currentData == nullptr) return;

//     // Check if a test note has been triggered from the UI
//     if (noteTriggered.exchange(false)) {
//         // Inject a MIDI Note On (Note 60, Velocity 1.0) into the buffer
//         midiMessages.addEvent(juce::MidiMessage::noteOn(1, 60, 1.0f), 0);
//     }

//     // Handle MIDI triggers
//     for (const auto metadata : midiMessages) {
//         auto msg = metadata.getMessage();
//         if (msg.isNoteOn()) {
//             // "Generalization": Trigger the sample as one large grain
//             grainEngine.triggerGrain(0, currentData->getBuffer().getNumSamples(), 0.5f, activeEnvelope.get());
//         }
//     }

//     // Process all active grains
//     grainEngine.process(buffer, currentData);
    
//     // Update playhead for UI (simplified to track the last triggered grain or engine state)
//     // For now, we'll keep this simple; in a full engine, you'd track the oldest active grain
// }

void PruvulazzuAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    buffer.clear();
    auto currentData = sampleManager.getCurrentBuffer();
    if (currentData == nullptr) return;

    // Handle Test Note Trigger
    if (noteTriggered.exchange(false)) isNoteActive = true;

    if (isNoteActive) {
        float density = *apvts.getRawParameterValue("density");
        int intervalSamples = static_cast<int>(getSampleRate() / (density / 1000.0f));
        
        samplesSinceLastGrain += buffer.getNumSamples();
        if (samplesSinceLastGrain >= intervalSamples) {
            float sizeMs = *apvts.getRawParameterValue("size");
            int len = static_cast<int>((sizeMs / 1000.0f) * getSampleRate());
            int start = juce::Random::getSystemRandom().nextInt(currentData->getBuffer().getNumSamples());
            
            grainEngine.triggerGrain(start, len, 0.5f, activeEnvelope.get());
            samplesSinceLastGrain = 0;
        }
    }
    grainEngine.process(buffer, currentData);
}

juce::AudioProcessorEditor* PruvulazzuAudioProcessor::createEditor() {
    return new PruvulazzuAudioProcessorEditor(*this);
}

// Mandatory JUCE requirement: this function must be implemented
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new PruvulazzuAudioProcessor();
}