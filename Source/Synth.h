//Synth.h===========================================================================

#pragma once

#include "Oscillator\OscParameters.h"
#include "Amplifier\AmpParameters.h"
#include "Oscillator\ModOscillator.h"
#include "Amplifier\Amplifier.h"

//���O��Ԃ�݂��Ă����A���R�͌�q
namespace synth 
{
	class Synth{
	public:
		//OscParamers, AmpParameters�֘A��������
		Synth(OscParameters&, AmpParameters&);

		void midiProcessing(MidiMessage&, double hostSampleRate);
		void renderOscData(int hostBufferSize);
		void ampProcessing(int hostBufferSize, double sampleRate);
        float getWaveDataBuffData(int buffPos);

	private:
		OscParameters& rOscParameters;
		AmpParameters& rAmpParameters;
		ModSineOscillator oscillator;
		Amplifier amplifier;
        //����Buffer
        //�T�C�Y�͓K�� (hostBufferSize���\���傫�߂ł���΂悢)
        //DAW�ɂ���邪�A�ő�ł��T�ː��S�T���v�����x�Ȃ̂ŏ\������
        float waveDataBuffer[48000];
	};
}

