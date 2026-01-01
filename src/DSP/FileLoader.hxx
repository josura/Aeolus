#pragma once
#include <juce_audio_formats/juce_audio_formats.h>
#include "SampleBufferData.hxx" // The ReferenceCountedBuffer we defined earlier

class FileLoader {
public:
    FileLoader() {
        formatManager.registerBasicFormats();
    }

    // Decodes file on the UI thread and returns a shared buffer
    SampleBufferData::Ptr loadFile(const juce::File& file) {
        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

        if (reader != nullptr) {
            juce::AudioSampleBuffer newBuffer(static_cast<int>(reader->numChannels), 
                                              static_cast<int>(reader->lengthInSamples));
            
            reader->read(&newBuffer, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);
            
            return new SampleBufferData(std::move(newBuffer), reader->sampleRate);
        }
        return nullptr;
    }

private:
    juce::AudioFormatManager formatManager;
};