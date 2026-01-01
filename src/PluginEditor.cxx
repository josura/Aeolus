#include "PluginEditor.hxx"

AeolusAudioProcessorEditor::AeolusAudioProcessorEditor(AeolusAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p),
      waveformVisualizer(processor.getFormatManager())
{
    setSize(600, 400);
    addAndMakeVisible(waveformVisualizer);
}

AeolusAudioProcessorEditor::~AeolusAudioProcessorEditor() {}

void AeolusAudioProcessorEditor::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void AeolusAudioProcessorEditor::resized() {
    waveformVisualizer.setBounds(getLocalBounds().reduced(20));
}

bool AeolusAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray&) { return true; }

void AeolusAudioProcessorEditor::filesDropped(const juce::StringArray& files, int, int) {
    juce::File file(files[0]);
    waveformVisualizer.setFile(file);
    auto newBuffer = loader.loadFile(file);
    processor.getSampleManager().updateCurrentBuffer(newBuffer);
}