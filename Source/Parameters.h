#pragma once

#include <JuceHeader.h>

const juce::ParameterID gainParamID{ "gain", 1 };
const juce::ParameterID panAngleParamID{ "panAngle", 1 };
const juce::ParameterID panRuleParamID{ "panRule", 1 };
const juce::ParameterID bypassParamID{ "bypass", 1 };

class Parameters
{
public:
	Parameters(juce::AudioProcessorValueTreeState& apvts);

	static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	void prepareToPlay(double sampleRate) noexcept;
	void reset() noexcept;
	void update() noexcept;
	void smoothen() noexcept;

	float gain = 0.0f;
	int panRule = 1;
	float panAngle = 0.0f;

private:
	juce::AudioParameterFloat* gainParam;
	juce::AudioParameterFloat* panAngleParam;
	juce::AudioParameterChoice* panRuleParam;
	juce::AudioParameterBool* bypassParam;

	juce::LinearSmoothedValue<float> gainSmoother;
};