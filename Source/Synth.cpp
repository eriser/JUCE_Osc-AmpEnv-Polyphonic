//Synth.cpp===========================================================================
#include "Synth.h"

using namespace synth;

Synth::Synth(OscParameters& oscParam, AmpParameters& ampParam)
	:rOscParameters(oscParam), rAmpParameters(ampParam),
	oscillator(), amplifier()
{}

void Synth::midiProcessing(MidiMessage& m, double hostSampleRate) {
	//midiOnを受け取ったらOscとAmpを初期化
	if (m.isNoteOn()) { 
		double midiNote_Hz = MidiMessage::getMidiNoteInHertz(m.getNoteNumber());
		oscillator.runOscillator(midiNote_Hz, hostSampleRate);
		amplifier.setGateOnState(rAmpParameters, hostSampleRate);
	}
	//midiOffを受け取っても、Oscの発振は止めない.
	//AmpはReleaseStateに.
	else { amplifier.setGateOffState(rAmpParameters, hostSampleRate); }
}

void Synth::renderOscData(int bufferSize) {
	//bufferSizeサンプル分Oscillatorに波形を生成させる
	for (int n = 0; n < bufferSize; n++) {
		oscillator.setParameters(rOscParameters);	//波形生成前にParameterを取得・適用
		waveDataBuffer[n] = oscillator.get();
		oscillator.phaseIncrement();
	}
}

void Synth::ampProcessing(int bufferSize, double sampleRate) {
	//bufferSizeサンプル分OscDataが書き込まれたBufferをAmpEvnに従ってVolume調整
	for (int n = 0; n < bufferSize; n++) {
		amplifier.applyAmpEvnProcessing(waveDataBuffer[n]);
		amplifier.setNextState(rAmpParameters, sampleRate);
	}
	//Ampが完全にOff状態であれば、波形生成させても無駄になるのでOscの発振を止める
	if (amplifier.isOffState()) { oscillator.stopOscillator(); }
}

float Synth::getWaveDataBuffData(int buffPos) {
    return waveDataBuffer[buffPos];
}