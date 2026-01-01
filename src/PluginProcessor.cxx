#include "PluginProcessor.hxx"

AeolusAudioProcessor::AeolusAudioProcessor() 
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    formatManager.registerBasicFormats();
}

AeolusAudioProcessor::~AeolusAudioProcessor() {}

void AeolusAudioProcessor::prepareToPlay (double, int) { playhead = 0; }
void AeolusAudioProcessor::releaseResources() {}

void AeolusAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto currentData = sampleManager.getCurrentBuffer();
    if (currentData == nullptr) return;

    const auto& sampleBuffer = currentData->getBuffer();
    
    auto* leftOut = buffer.getWritePointer(0);
    auto* rightOut = buffer.getWritePointer(1);
    const auto* sampleIn = sampleBuffer.getReadPointer(0);

    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        if (playhead < sampleBuffer.getNumSamples()) {
            float sample = sampleIn[playhead];
            leftOut[i] += sample;
            if (totalNumOutputChannels > 1) rightOut[i] += sample;
            playhead++;
        } else {
            playhead = 0; 
        }
    }
}

juce::AudioProcessorEditor* AeolusAudioProcessor::createEditor() {
    // We will fix the Editor class name in the next step
    return nullptr; 
}

// Mandatory JUCE requirement: this function must be implemented
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new AeolusAudioProcessor();
}