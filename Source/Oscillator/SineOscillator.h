//SineOscillator.h===========================================================================
#pragma once

//�~������sin()�֐��Amidi�̂��ꂱ��𗘗p�������̂�
#include "JuceHeader.h"

class SineOscillator {
public:
	void runOscillator(double frequency, double sampleRate);	//midiOn���͂����������̏���
	void stopOscillator();			//midiOff���͂����������̏���
	virtual double get();			//���݃T���v�����Ԃ̔g�`�f�[�^���擾
	void phaseIncrement();			//1�T���v�����Ԑi�߂�

	void changeFrequency(double newVal);	//�ǉ�

protected:
	bool _isRunning = false;	//���݁AOsc��MidiOn��ԂȂ̂�Off��ԂȂ̂�
	double _n = 0;				//�g�`�������J�n���Ă���̌o�߃T���v������ (����������̂Ő����l�ȊO����肤��)
	double _f = 440;			//���g��
	double _fs = 48000.0;		//�T���v�����O���g��
	double _p = 100;			//���� (�T���v������)

private:
	//NVI�C�f�B�I��
	virtual void runOscillatorNVI() {};
	virtual void stopOscillatorNVI() {};
	virtual void phaseIncrementNVI() {};
};