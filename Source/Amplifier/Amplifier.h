#pragma once

#include "AmpParameters.h"

class Amplifier {
public:
	//MidiOn�ɂȂ������̏���
	void setGateOnState(AmpParameters&, double sampleRate);
	//MidiOff�ɂȂ������̏���
	void setGateOffState(AmpParameters&, double sampleRate);
	//�A���vEvn�̏�Ԃɉ����āA�n���ꂽ�I�[�f�B�I�̃{�����[�����i��
	void applyAmpEvnProcessing(float& audioSignal);
	//�A���vEvn���̎��Ԃ�i�߂�E��Ԃ̐؂�ւ�����
	void setNextState(AmpParameters&, double sampleRate);
	bool isOffState();

private:
	//AmpEvn�̏��
	enum EvnState {
		Attack = 0, Decay, Sustain, Release, Off
	};

	int _state = Off;		//���݃T���v�����Ԃ�AmpEvn�̏��
	int _n = 0;				//���݂̏�ԂɈڂ��Ă���̌o�߃T���v������
	int _duration = 1;		//���̏�ԂɈڂ�܂ł̃T���v������
	double _gain = 0.0;		//�ǂ̒��x���ʂ��i�邩
	double _sloop = 0.0;	//Evn�J�[�u�̌X��
};