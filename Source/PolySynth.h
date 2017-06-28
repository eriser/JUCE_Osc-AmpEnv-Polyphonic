#pragma once

#include "Synth.h"
#include <list>

namespace synth 
{
    class PolySynth {
    public:
        PolySynth(AudioProcessor&);

        void midiOnProcessing(MidiMessage&, double sampleRate);
        void midiOffProcessing(MidiMessage&, double sampleRate);
        void waveRender(int buffSize, double sampleRate);
        void mergeWaveData(float* hostBuffer, int buffSize);

    private:
        //Parameterは各MonoSynthに共通なんので、
        //包含元であるPolySynthに1つだけインスタンスを生成させ、
        //各MonoSynthにはこれらの参照を渡すように変更
        OscParameters oscParam;
        AmpParameters ampParam;

        struct VoiceState {
            VoiceState(int voiceId) : voiceId(voiceId){};
            const int voiceId;
            int midiNoteNum = -1;   //-1をmidiNoteOffとする
        };

        static const int maxNumVoice = 8;   //最大同時発音数
        std::list<VoiceState> lisOnVoiceState;
        std::list<VoiceState> lisOffVoiceState;
        std::vector<Synth> vecVoice;    //MonoSynth (Osc, Ampが実装されている)

        typedef std::list<VoiceState>::iterator lItr;
        typedef std::vector<Synth>::iterator vItr;
    };

    void doProcessing(PolySynth&, AudioSampleBuffer&, MidiBuffer&, double sampleRate);
}