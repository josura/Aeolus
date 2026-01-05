// DSP/Envelopes.hxx
#pragma once
#include <cmath>

class Envelope {
public:
    virtual ~Envelope() = default;
    
    // Returns a gain multiplier between 0.0 and 1.0
    virtual float getLevel(int currentSample, int duration) = 0;
};

class LinearEnvelope : public Envelope {
public:
    float getLevel(int currentSample, int duration) override {
        float halfWay = duration * 0.5f;
        if (currentSample < halfWay)
            return (float)currentSample / halfWay;
        return (float)(duration - currentSample) / halfWay;
    }
};

class ParabolicEnvelope : public Envelope {
public:
    float getLevel(int currentSample, int duration) override {
        // Simple x(1-x) normalized parabola
        float normalizedPos = (float)currentSample / (float)duration;
        return 4.0f * normalizedPos * (1.0f - normalizedPos);
    }
};