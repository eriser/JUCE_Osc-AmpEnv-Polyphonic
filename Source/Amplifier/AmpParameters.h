#pragma once

#include "..\Utility\ParametersBase.h"

class AmpParameters : public ParametersBase {
public:
	AmpParameters(AudioProcessor&);

	enum ParameterId {Attack = 0, Decay, Sustaine, Release, totalNumParams};
};