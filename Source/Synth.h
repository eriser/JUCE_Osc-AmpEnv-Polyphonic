//Synth.h===========================================================================

#pragma once

#include "Oscillator\OscParameters.h"
#include "Amplifier\AmpParameters.h"
#include "Oscillator\ModOscillator.h"
#include "Amplifier\Amplifier.h"

//名前空間を設けておく、理由は後述
namespace synth 
{
	class Synth{
	public:
		//OscParamers, AmpParametersへ連鎖させる
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
        //内部Buffer
        //サイズは適当 (hostBufferSizeより十分大きめであればよい)
        //DAWにもよるが、最大でも概ね数百サンプル程度なので十分かと
        float waveDataBuffer[48000];
	};
}

