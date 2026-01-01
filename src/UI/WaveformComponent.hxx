#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>

class WaveformComponent : public juce::Component, public juce::ChangeListener {
public:
    WaveformComponent(juce::AudioFormatManager& formatManager)
        : thumbnail(512, formatManager, thumbnailCache) 
    {
        thumbnail.addChangeListener(this);
    }

    void setFile(const juce::File& file) {
        thumbnail.setSource(new juce::FileInputSource(file));
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::cyan);

        if (thumbnail.getNumChannels() > 0) {
            thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
        } else {
            g.drawText("Drop an audio file here", getLocalBounds(), juce::Justification::centred);
        }
    }

    void changeListenerCallback(juce::ChangeBroadcaster*) override { repaint(); }

private:
    juce::AudioThumbnailCache thumbnailCache{ 5 };
    juce::AudioThumbnail thumbnail;
};