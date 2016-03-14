/*
 ==============================================================================
 GRNLR VST IMPLEMENTATION
 Concept:
 > quasisyncronous granular synthesis engine based on a supercollider prototype
 > support for an approach to granular synthesis inspired by Curtis Roads book
 "microsound"
 
 !! TODO !!
 STUCTURE:
    > a grainsynth class that handles rendering the block of a single grain
 ENGINE:
    > allow for grains to be shorter than buffersize
    > allow for grains to start and stop inside a running buffer
    > grains have start / end
    > an asynchronous massaging system between the process function and things
      like grain prosition in envelope etc.
 SCHEDULER:
    > should the scheduler run on a different thread than the process application?
    > functionality should be something like SuperCollider's pattern system:
      > scheduling is based on inter-onset times, there is only ever one event
        scheduled at a time
        for example:
	the first event is scheduled 2 seconds into the future
	at the time when this event is scheduled the scheduler is woken up
	and schedules the next event into the future
      > the scheduling is done via creating and destroying tuples of Grain-Objects
        and times on the GrainStack
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Grnlr_kleinAudioProcessor::Grnlr_kleinAudioProcessor()
{
}

Grnlr_kleinAudioProcessor::~Grnlr_kleinAudioProcessor()
{
}

//==============================================================================
const String Grnlr_kleinAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Grnlr_kleinAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Grnlr_kleinAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool Grnlr_kleinAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double Grnlr_kleinAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Grnlr_kleinAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int Grnlr_kleinAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Grnlr_kleinAudioProcessor::setCurrentProgram (int index)
{
    
}

const String Grnlr_kleinAudioProcessor::getProgramName (int index)
{
    return String();
}

void Grnlr_kleinAudioProcessor::changeProgramName (int index, const String& newName)
{
    
}

//==============================================================================

void Grnlr_kleinAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // initialize some values, maybe some of this stuff belongs in the constructor (?)
    position = 0;
    lengthRatio = 0;
    updateValues();
    startTime = std::chrono::steady_clock::now();
    
}

void Grnlr_kleinAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void Grnlr_kleinAudioProcessor::applyEnvelope (AudioSampleBuffer& buffer)
{
    // simple envelope over one loopcycle, replace this with a more sofisticated approach later on
    for (int channel=0; channel<buffer.getNumChannels(); ++channel)
    {
        buffer.applyGainRamp(channel, 0, buffer.getNumSamples()/2, 0, 1 );
        buffer.applyGainRamp(channel, buffer.getNumSamples()/2, buffer.getNumSamples(), 1, 0);
    }
}

void Grnlr_kleinAudioProcessor::updateValues ()
{
    // update some values asynchronously with the audio blocks
    lengthInSamples         = lengthRatio * fileBuffer.getNumSamples();
    positionOffsetInSamples = positionOffset * fileBuffer.getNumSamples();
}

//==============================================================================
double Grnlr_kleinAudioProcessor::checkTime()
{
    // check the time every so often
    // TODO: figure out if the time should be checked every audio block or at another interval
    using namespace std::chrono;
    steady_clock::time_point timeNow = steady_clock::now();
    
    time = duration_cast<duration<double>>(timeNow - startTime);
    return time.count();
}

void Grnlr_kleinAudioProcessor::schedule(int startPosition, int length, int time)
{
    //waitingStack.push(startPosition, length, time);
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}


/**
 PROCESS FUNCTION
 do everything that needs to happen every audio block,
 check the stack for all grains that are playing or should start this block:
   1. check the last grain on the waiting stack if it is playing in this audio block
      if it is remove it from the waiting stack and move it to the playing stack
   2. render all the grains on the playing stack and add them together
   3. if a grain in the playing stack is ending this block remove it from the playing stack
      after rendering (and destroy it?)
 */
void Grnlr_kleinAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
  double time = checkTime();
  grain.process(buffer, fileBuffer, positionOffsetInSamples, lengthInSamples);
  applyEnvelope(buffer);
  updateValues();
}

//==============================================================================
bool Grnlr_kleinAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Grnlr_kleinAudioProcessor::createEditor()
{
    return new Grnlr_kleinAudioProcessorEditor (*this);
}

//==============================================================================
void Grnlr_kleinAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Grnlr_kleinAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Grnlr_kleinAudioProcessor();
}
