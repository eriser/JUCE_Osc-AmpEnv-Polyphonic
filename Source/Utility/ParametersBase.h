#pragma once

#include "JuceHeader.h"
#include <vector>

class ParametersBase {
public:
	float get(int parameterId);

protected:
	ParametersBase(AudioProcessor&, int totalNumParams);
	void resisterNewParameter(AudioParameterFloat*);

private:
	AudioProcessor& _processor;
	std::vector<AudioParameterFloat*> _vecParameterPtr;
};