#include "Amplifier.h"

void Amplifier::setGateOnState(AmpParameters& p, double sampleRate) {
	//各種パラメータを初期化
	_state = EvnState::Attack;
	_n = 0;
	_duration = p.get(AmpParameters::Attack) * sampleRate + 1;
	_gain = 0.0;
	_sloop = 1.0 / _duration;
}

void Amplifier::setGateOffState(AmpParameters& p, double sampleRate) {
	_state = EvnState::Release;
	_n = 0;
	_duration = p.get(AmpParameters::Release) * sampleRate + 1;
	_sloop = -_gain / _duration;
}

void Amplifier::applyAmpEvnProcessing(float& audioSignal) {
	audioSignal = audioSignal * _gain;
}

void Amplifier::setNextState(AmpParameters& p, double sampleRate) {
	//Off状態・Sustain状態から時間経過による次状態への遷移は起こらないので、時間を進める必要無し
	//Off->AttackはMidiOnの時のみ、Sustain->ReleaseはMidiOffの時のみ
	if (_state == EvnState::Off || _state == EvnState::Sustain) {
		return;
	}

	if (++_n >= _duration) {
		//Evnの状態を進め、経過時間をリセット
		_state++;
		_n = 0;

		if (_state == EvnState::Decay) {
			_duration = p.get(AmpParameters::Decay) * sampleRate + 1;
			_gain = 1.0;
			_sloop = -(_gain - p.get(AmpParameters::Sustaine)) / _duration;
		}
		else if (_state == EvnState::Sustain) {
			_duration = 1;
			_gain = p.get(AmpParameters::Sustaine);
			_sloop = 0.0;
		}
		//OffState
		else {
			_duration = 1;
			_gain = 0.0;
			_sloop = 0.0;
		}
	}
	//今回はEvnカーブが直線で傾きが一定なので、
	//状態遷移時に計算した傾きを1サンプル更新毎に加算し続けるだけでOK
	_gain += _sloop;
}

bool Amplifier::isOffState() {
	if (_state == EvnState::Off) { return true; }
	else { return false; }
}