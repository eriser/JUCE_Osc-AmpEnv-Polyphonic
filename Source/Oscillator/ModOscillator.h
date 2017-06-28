//ModOscillator.h===========================================================================
#pragma once

#include "OscParameters.h"
#include "SineOscillator.h"

//modSineOscillator���g��carrier�Ƃ��ē���
//protected�p�����������o�ϐ��́A�S��carrier�̃p�����[�^
class ModSineOscillator: public SineOscillator{
public:
	void setParameters(OscParameters&);
	double get() override;

private:
	//modulatorSine�̐U�镑�����L�q
	void runOscillatorNVI() override;
	void stopOscillatorNVI() override;
	void phaseIncrementNVI() override;

	SineOscillator _modulatorSine;

	//���[�U�[�ݒ�\�Ȋe��Parameter
	int _modType = OscParameters::AM;
	double _modRate = 1;
	float _modAmount = 0.f;
};