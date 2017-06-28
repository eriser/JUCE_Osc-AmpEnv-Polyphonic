#include "ParametersBase.h"

ParametersBase::ParametersBase(AudioProcessor& processor, int totalNumParams)
	:_processor(processor)
{
	_vecParameterPtr.reserve(totalNumParams);
}

float ParametersBase::get(int parameterId) {
	return _vecParameterPtr.at(parameterId)->get();
}

void ParametersBase::resisterNewParameter(AudioParameterFloat* pNewParam) {
	_vecParameterPtr.push_back(pNewParam);
	_processor.addParameter(pNewParam);
}