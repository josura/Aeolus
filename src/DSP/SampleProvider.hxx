#pragma once
#include <JuceHeader.h>

/**
 * A thread-safe, shared buffer for the sampler.
 * Uses ReferenceCountedObject so the Audio Thread can hold onto it
 * even if the user loads a new file mid-process.
 */
class SampleBufferData : public juce::ReferenceCountedObject {
public:
    using Ptr = juce::ReferenceCountedObjectPtr<SampleBufferData>;

    SampleBufferData(juce::AudioSampleBuffer&& bufferToAdopt, double sampleRate)
        : buffer(std::move(bufferToAdopt)), sourceSampleRate(sampleRate) {}

    const juce::AudioSampleBuffer& getBuffer() const { return buffer; }
    double getSampleRate() const { return sourceSampleRate; }

private:
    juce::AudioSampleBuffer buffer;
    double sourceSampleRate;
};