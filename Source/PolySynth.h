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
        //Parameter�͊eMonoSynth�ɋ��ʂȂ�̂ŁA
        //��܌��ł���PolySynth��1�����C���X�^���X�𐶐������A
        //�eMonoSynth�ɂ͂����̎Q�Ƃ�n���悤�ɕύX
        OscParameters oscParam;
        AmpParameters ampParam;

        struct VoiceState {
            VoiceState(int voiceId) : voiceId(voiceId){};
            const int voiceId;
            int midiNoteNum = -1;   //-1��midiNoteOff�Ƃ���
        };

        static const int maxNumVoice = 8;   //�ő哯��������
        std::list<VoiceState> lisOnVoiceState;
        std::list<VoiceState> lisOffVoiceState;
        std::vector<Synth> vecVoice;    //MonoSynth (Osc, Amp����������Ă���)

        typedef std::list<VoiceState>::iterator lItr;
        typedef std::vector<Synth>::iterator vItr;
    };

    void doProcessing(PolySynth&, AudioSampleBuffer&, MidiBuffer&, double sampleRate);
}