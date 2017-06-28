#include "OscParameters.h"

OscParameters::OscParameters(AudioProcessor& ap)
	:ParametersBase(ap, totalNumParams)
{
	//enum ParameterId‚Å’è‹`‚µ‚½‡‚É‘}“ü‚µ‚Ä‚¢‚­
	resisterNewParameter(new AudioParameterFloat("ModType", "ModType", ModTypeId::AM, ModTypeId::FM, ModTypeId::AM));
	resisterNewParameter(new AudioParameterFloat("ModRate", "ModRate", 1.f, 10.f, 1.f));
	resisterNewParameter(new AudioParameterFloat("ModAmount", "ModAmount", 0.f, 10.f, 0.f));
}
