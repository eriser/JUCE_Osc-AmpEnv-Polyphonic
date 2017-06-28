//SineOscillator.cpp===========================================================

#include "SineOscillator.h"

void SineOscillator::runOscillator(double frequency, double sampleRate) {
	//�e��p�����[�^��������
	_isRunning = true;
	_n = 0;
	_f = frequency;
	_fs = sampleRate;
	_p = sampleRate / _f;

	runOscillatorNVI();
}

void SineOscillator::stopOscillator() {
	_isRunning = false;
	
	stopOscillatorNVI();
}

double SineOscillator::get() {
	if (!_isRunning) { return 0.0; }

	//sin(w * t) = sin(2 * pi * f * n / fs)
	return sin(2.0 * double_Pi * _n / _p);
}

void SineOscillator::phaseIncrement() {
	if (!_isRunning) { return; }

	//1�T���v�����Ԑi�߂āA�K�v�ɉ����Ď�����
	if (++_n >= _p) { _n = _n - int(_n / _p) * _p; }

	phaseIncrementNVI();
}

void SineOscillator::changeFrequency(double newVal) {
	_f = newVal;
	_p = _fs / _f;
}