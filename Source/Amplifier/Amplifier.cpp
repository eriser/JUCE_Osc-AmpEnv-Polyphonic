#include "Amplifier.h"

void Amplifier::setGateOnState(AmpParameters& p, double sampleRate) {
	//�e��p�����[�^��������
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
	//Off��ԁESustain��Ԃ��玞�Ԍo�߂ɂ�鎟��Ԃւ̑J�ڂ͋N����Ȃ��̂ŁA���Ԃ�i�߂�K�v����
	//Off->Attack��MidiOn�̎��̂݁ASustain->Release��MidiOff�̎��̂�
	if (_state == EvnState::Off || _state == EvnState::Sustain) {
		return;
	}

	if (++_n >= _duration) {
		//Evn�̏�Ԃ�i�߁A�o�ߎ��Ԃ����Z�b�g
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
	//�����Evn�J�[�u�������ŌX�������Ȃ̂ŁA
	//��ԑJ�ڎ��Ɍv�Z�����X����1�T���v���X�V���ɉ��Z�������邾����OK
	_gain += _sloop;
}

bool Amplifier::isOffState() {
	if (_state == EvnState::Off) { return true; }
	else { return false; }
}