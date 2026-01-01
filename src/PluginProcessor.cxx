// Inside PluginProcessor.cpp - processBlock()
void FragmentAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // 1. Clear the output buffer
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // 2. Get the current sample data
    auto currentData = sampleManager.getCurrentBuffer();
    if (currentData == nullptr) return;

    const auto& sampleBuffer = currentData->getBuffer();
    
    // 3. Simple Playback Loop (The "Single Grain" Test)
    auto* leftOut = buffer.getWritePointer(0);
    auto* rightOut = buffer.getWritePointer(1);
    const auto* sampleIn = sampleBuffer.getReadPointer(0); // Assuming mono for now

    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        if (playhead < sampleBuffer.getNumSamples()) {
            float sample = sampleIn[playhead];
            leftOut[i] += sample;
            rightOut[i] += sample;
            playhead++;
        } else {
            playhead = 0; // Loop for testing
        }
    }
}