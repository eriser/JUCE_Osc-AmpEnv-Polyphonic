#include "AmpParameters.h"

AmpParameters::AmpParameters(AudioProcessor& processor)
	:ParametersBase(processor, totalNumParams)
{
	//enumÇ≈íËã`ÇµÇΩèáÇ…ìoò^
	resisterNewParameter(new AudioParameterFloat("Attack_sec", "Attack_sec", 0.f, 1.f, 0.f));
	resisterNewParameter(new AudioParameterFloat("Decay_sec", "Decay_sec", 0.3f, 5.f, 0.3f));
	resisterNewParameter(new AudioParameterFloat("Sustain", "Sustain", 0.f, 1.f, 0.f));
	resisterNewParameter(new AudioParameterFloat("Release_sec", "Release_sec", 0.3f, 5.f, 0.3f));
}

