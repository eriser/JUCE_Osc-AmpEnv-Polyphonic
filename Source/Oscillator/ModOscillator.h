//ModOscillator.h===========================================================================
#pragma once

#include "OscParameters.h"
#include "SineOscillator.h"

//modSineOscillator自身はcarrierとして働く
//protected継承したメンバ変数は、全てcarrierのパラメータ
class ModSineOscillator: public SineOscillator{
public:
	void setParameters(OscParameters&);
	double get() override;

private:
	//modulatorSineの振る舞いを記述
	void runOscillatorNVI() override;
	void stopOscillatorNVI() override;
	void phaseIncrementNVI() override;

	SineOscillator _modulatorSine;

	//ユーザー設定可能な各種Parameter
	int _modType = OscParameters::AM;
	double _modRate = 1;
	float _modAmount = 0.f;
};