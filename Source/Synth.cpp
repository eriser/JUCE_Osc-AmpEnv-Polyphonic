//Synth.cpp===========================================================================
#include "Synth.h"

using namespace synth;

Synth::Synth(OscParameters& oscParam, AmpParameters& ampParam)
	:rOscParameters(oscParam), rAmpParameters(ampParam),
	oscillator(), amplifier()
{}

void Synth::midiProcessing(MidiMessage& m, double hostSampleRate) {
	//midiOn���󂯎������Osc��Amp��������
	if (m.isNoteOn()) { 
		double midiNote_Hz = MidiMessage::getMidiNoteInHertz(m.getNoteNumber());
		oscillator.runOscillator(midiNote_Hz, hostSampleRate);
		amplifier.setGateOnState(rAmpParameters, hostSampleRate);
	}
	//midiOff���󂯎���Ă��AOsc�̔��U�͎~�߂Ȃ�.
	//Amp��ReleaseState��.
	else { amplifier.setGateOffState(rAmpParameters, hostSampleRate); }
}

void Synth::renderOscData(int bufferSize) {
	//bufferSize�T���v����Oscillator�ɔg�`�𐶐�������
	for (int n = 0; n < bufferSize; n++) {
		oscillator.setParameters(rOscParameters);	//�g�`�����O��Parameter���擾�E�K�p
		waveDataBuffer[n] = oscillator.get();
		oscillator.phaseIncrement();
	}
}

void Synth::ampProcessing(int bufferSize, double sampleRate) {
	//bufferSize�T���v����OscData���������܂ꂽBuffer��AmpEvn�ɏ]����Volume����
	for (int n = 0; n < bufferSize; n++) {
		amplifier.applyAmpEvnProcessing(waveDataBuffer[n]);
		amplifier.setNextState(rAmpParameters, sampleRate);
	}
	//Amp�����S��Off��Ԃł���΁A�g�`���������Ă����ʂɂȂ�̂�Osc�̔��U���~�߂�
	if (amplifier.isOffState()) { oscillator.stopOscillator(); }
}

float Synth::getWaveDataBuffData(int buffPos) {
    return waveDataBuffer[buffPos];
}