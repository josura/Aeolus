#pragma once
#include <vector>
#include "Grain.hxx"
#include "SampleBufferData.hxx"

class GrainEngine {
public:
    GrainEngine() = default;

    void prepare(int maxGrains) {
        grainPool.resize(maxGrains);
    }

    // Triggers a new grain from the pool
    void triggerGrain(int start, int length, float pan, Envelope* env) {
        for (auto& grain : grainPool) {
            if (!grain.isActive()) {
                grain.init(start, length, pan, env);
                return;
            }
        }
    }

    void process(juce::AudioBuffer<float>& outputBuffer, const SampleBufferData::Ptr& sampleData) {
        if (sampleData == nullptr) return;

        const auto& sourceBuffer = sampleData->getBuffer();
        const float* sourceRaw = sourceBuffer.getReadPointer(0);
        int sourceLen = sourceBuffer.getNumSamples();

        auto* leftOut = outputBuffer.getWritePointer(0);
        auto* rightOut = outputBuffer.getWritePointer(1);

        for (int s = 0; s < outputBuffer.getNumSamples(); ++s) {
            float leftSum = 0.0f;
            float rightSum = 0.0f;

            for (auto& grain : grainPool) {
                if (grain.isActive()) {
                    float sample = grain.process(sourceRaw, sourceLen);
                    float p = grain.getPan();
                    
                    // Simple constant-power panning
                    leftSum += sample * std::sqrt(1.0f - p);
                    rightSum += sample * std::sqrt(p);
                }
            }
            leftOut[s] += leftSum;
            rightOut[s] += rightSum;
        }
    }

    void stopAll() {
        for (auto& grain : grainPool) grain.init(0, 0, 0, nullptr); 
    }

private:
    std::vector<Grain> grainPool;
};