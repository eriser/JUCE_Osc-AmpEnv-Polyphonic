#pragma once

#include "..\Utility\ParametersBase.h"

class OscParameters: public ParametersBase{
public:
	OscParameters(AudioProcessor&);

	enum ParameterId { modType = 0, modRate, modAmount, totalNumParams };
	enum ModTypeId { AM = 0, FM };
};