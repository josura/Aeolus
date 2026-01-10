// DSP/Grain.hxx
#pragma once
#include "Envelopes.hxx"

class Grain {
public:
    Grain() = default;

    void init(int start, int len, float p, Envelope* env) {
        startSample = start;
        duration = len;
        pan = p;
        currentSample = 0;
        active = true;
        currentEnvelope = env;
    }

    // Process one sample and return the result
    float process(const float* sourceData, int sourceLength) {
        if (!active || currentEnvelope == nullptr) return 0.0f;

        float envLevel = currentEnvelope->getLevel(currentSample, duration);
        int readPos = (startSample + currentSample) % sourceLength;
        
        float out = sourceData[readPos] * envLevel;

        currentSample++;
        if (currentSample >= duration) active = false;

        return out;
    }

    // Returns the current position in the sample (0.0 to 1.0)
    // Returns -1.0 if the grain is inactive
    float getNormalizedPosition(int totalSampleLength) const {
        if (!active || totalSampleLength == 0) return -1.0f;
        return static_cast<float>(currentSampleIndex) / static_cast<float>(totalSampleLength);
    }

    bool isActive() const { return active; }
    float getPan() const { return pan; }

private:
    int startSample = 0;
    int duration = 0;
    int currentSample = 0;
    int currentSampleIndex = 0; // The absolute index in the source buffer
    bool active = false;
    float pan = 0.5f;
    bool active = false;
    Envelope* currentEnvelope = nullptr; 
};