#include "ModOscillator.h"

void ModSineOscillator::setParameters(OscParameters& p) {
	_modType = (int)p.get(OscParameters::modType);
	_modRate = (int)p.get(OscParameters::modRate);
	_modAmount = p.get(OscParameters::modAmount);

	//modulatorFrequency = _modRate * carrierFrequency
	double modulatorFrequency = _modRate * _f;
	_modulatorSine.changeFrequency(modulatorFrequency);
}

double ModSineOscillator::get() {
	if (!_isRunning) { return 0.0; }

	double y = 0.0;	//Output
	double modSineSignal = _modAmount * _modulatorSine.get();

	//AM処理
	if (_modType == OscParameters::AM) {
		y = (1.0 + modSineSignal) * sin(2.0 * double_Pi *_n / _p);
		y = y / (1.0 + _modAmount);	//Normalize
	}
	//FM処理
	else { 
		y = sin(2.0 * double_Pi * _n / _p + modSineSignal);
	}

	return y;
}

void ModSineOscillator::runOscillatorNVI() {
	//modulatorもcarrierの周波数で初期化
	//applyParameter()で適切な周波数へ変更する
	_modulatorSine.runOscillator(_f, _fs);
}

void ModSineOscillator::stopOscillatorNVI() { _modulatorSine.stopOscillator(); }

void ModSineOscillator::phaseIncrementNVI() { _modulatorSine.phaseIncrement(); }