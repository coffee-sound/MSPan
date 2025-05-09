#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);  // parameter does not exist or wrong type
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, panAngleParamID, panAngleParam);
    castParameter(apvts, panRuleParamID, panRuleParam);
    castParameter(apvts, bypassParamID, bypassParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // parameters that should be added to the apvts
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        gainParamID,
        "Output Gain",
        juce::NormalisableRange<float>{ -12.0f, 12.0f },
        0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        panAngleParamID,
        "Pan Angle",
        juce::NormalisableRange<float>{ -100.0f, 100.0f },
        0.0f));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        panRuleParamID,
        "Pan Rule",
        juce::StringArray{ "linear", "balanced" },
        1));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        bypassParamID,
        "Bypass",
        false));

    return layout;
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
}

void Parameters::reset() noexcept
{
    gain = 0.0f;
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));

    panRule = 1;
    panAngle = 0.0f;
}

void Parameters::update() noexcept
{
    float gainInDecibels = gainParam->get();
    float newGain = juce::Decibels::decibelsToGain(gainInDecibels);
    gainSmoother.setTargetValue(newGain);

    panRule = panRuleParam->getIndex();
    panAngle = panAngleParam->get();
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
}