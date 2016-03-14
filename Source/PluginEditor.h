/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Grnlr_kleinAudioProcessorEditor  : public  AudioProcessorEditor,
                                         private Slider::Listener,
                                         Button::Listener
{
public:
    Grnlr_kleinAudioProcessorEditor (Grnlr_kleinAudioProcessor&);
    ~Grnlr_kleinAudioProcessorEditor();


    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked(Button* button) override;
    void sliderValueChanged(Slider* slider) override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Grnlr_kleinAudioProcessor& processor;

    void openButtonClicked();
    void clearButtonClicked();

    AudioFormatManager formatManager;

    TextButton openButton;
    TextButton clearButton;

    Slider lengthSlider;
    Slider positionSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Grnlr_kleinAudioProcessorEditor)
};

#endif  // PLUGINEDITOR_H_INCLUDED
