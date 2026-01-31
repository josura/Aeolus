#include "PluginProcessor.hxx"
#include "PluginEditor.hxx"

PruvulazzuAudioProcessor::PruvulazzuAudioProcessor() 
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    formatManager.registerBasicFormats();
}

PruvulazzuAudioProcessor::~PruvulazzuAudioProcessor() {}

void PruvulazzuAudioProcessor::prepareToPlay (double, int) { playhead = 0; }
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

void PruvulazzuAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    auto currentData = sampleManager.getCurrentBuffer();
    if (currentData == nullptr) {
        buffer.clear();
        return;
    }

    // Check if a test note has been triggered from the UI
    if (noteTriggered.exchange(false)) {
        // Inject a MIDI Note On (Note 60, Velocity 1.0) into the buffer
        midiMessages.addEvent(juce::MidiMessage::noteOn(1, 60, 1.0f), 0);
    }

    // 1. MIDI Handling & Scheduling
    for (const auto metadata : midiMessages) {
        auto msg = metadata.getMessage();
        if (msg.isNoteOn()) {
            isNoteActive = true;
            samplesSinceLastGrain = grainIntervalSamples; // Force immediate trigger
        } else if (msg.isNoteOff()) {
            isNoteActive = false;
        }
    }

    // 2. The Scheduler: Firing "Pruvulazzu" (Dust)
    if (isNoteActive) {
        samplesSinceLastGrain += buffer.getNumSamples();
        
        if (samplesSinceLastGrain >= grainIntervalSamples) {
            // Trigger a new grain! 
            // Randomized start pos and length for that "dust" effect
            int totalSamples = currentData->getBuffer().getNumSamples();
            int randomStart = juce::Random::getSystemRandom().nextInt(totalSamples);
            int length = 4410 * 2; // 200ms grains
            float randomPan = juce::Random::getSystemRandom().nextFloat();

            grainEngine.triggerGrain(randomStart, length, randomPan, nullptr);
            
            samplesSinceLastGrain = 0;
        }
    }

    // 3. DSP Processing
    buffer.clear();
    grainEngine.process(buffer, currentData);
}

juce::AudioProcessorEditor* PruvulazzuAudioProcessor::createEditor() {
    return new PruvulazzuAudioProcessorEditor(*this);
}

// Mandatory JUCE requirement: this function must be implemented
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new PruvulazzuAudioProcessor();
}