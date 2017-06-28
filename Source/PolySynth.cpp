#include "PolySynth.h"

using namespace synth;

PolySynth::PolySynth(AudioProcessor& processor)
    :oscParam(processor),
    ampParam(processor),
    //Synth�́AOscParameter, ampParameter�̎Q�Ƃ��󂯎��悤�ɕύX
    vecVoice(maxNumVoice, Synth(oscParam, ampParam))
{
    for (int n = 0; n < maxNumVoice; n++) {
        //VoiceState�ɌŗL��Id��^����Voice�����R���X�g���N�g
        //������Ԃł́A���ׂĂ�Voice��Off
        //0 ~ (maxNumVoice - 1)��Id��vecVoice[n]��n�ɑΉ�����
        lisOffVoiceState.push_back(VoiceState(n));
    }
}

void PolySynth::midiOnProcessing(MidiMessage& m, double sampleRate) {
    int inputNoteNum = m.getNoteNumber();
    int changeVoiceId;  //noteOn�ɂ���VoiceId
    lItr itr;   //�ύX�ΏۂƂȂ�VoiceState���w��
    //��̃{�C�X�����݂���
    if (!lisOffVoiceState.empty()) {
        //�擪�̋�{�C�X��ύX�ΏۂɎ��
        itr = lisOffVoiceState.begin();
        changeVoiceId = itr->voiceId;
        itr->midiNoteNum = inputNoteNum;
        //�ύX�̂�����VoiceState��lisOnVoiceState�̖����ֈڂ�
        lisOnVoiceState.push_back(VoiceState(*itr));
        lisOffVoiceState.pop_front();
    }
    //��̃{�C�X�����݂��Ȃ��ꍇ�́AOn��Ԃɂ���Voice�̈�ԌÂ����̂��㏑��
    else {
        //push_back�ŋl�߂Ă���̂�list�̐擪����ԌÂ�Voice�ɂȂ�
        //��́A��Ɗ�{�ꏏ (�R�[�h�̏d�����C�ɂȂ邪�A����͏����̗���̂킩��₷���D���)
        itr = lisOnVoiceState.begin();
        changeVoiceId = itr->voiceId;
        itr->midiNoteNum = inputNoteNum;
        lisOnVoiceState.push_back(VoiceState(*itr));
        lisOnVoiceState.pop_front();    //��ƈႤ�̂Œ���
    }
    //�Ή�����Voice��On��Ԃ�
    vecVoice[changeVoiceId].midiProcessing(m, sampleRate);
}

void PolySynth::midiOffProcessing(MidiMessage& m, double sampleRate) {
    int inputNoteNum = m.getNoteNumber();
    int changeVoiceId;  //noteOff�ɂ���VoiceId
    lItr itr = lisOnVoiceState.begin();   //�ύX�ΏۂƂȂ�VoiceState���w��

    //midiOff�ɂȂ�ׂ�NoteNumber��ێ�����VoiceState��T��
    while (itr != lisOnVoiceState.end()) {
        //�ύX���ׂ��{�C�X������������A���̃{�C�X��lisVoiceOff�����Ɉڂ�
        if (itr->midiNoteNum == inputNoteNum) {
            changeVoiceId = itr->voiceId;
            itr->midiNoteNum = -1;
            lisOffVoiceState.push_back(VoiceState(*itr));
            lisOnVoiceState.erase(itr);
            break;
        }
        itr++;
    }
    //�Ή�����Voice��Off��Ԃ�
    vecVoice[changeVoiceId].midiProcessing(m, sampleRate);
}

void PolySynth::waveRender(int buffSize, double sampleRate) {
    vItr itr = vecVoice.begin();

    while (itr != vecVoice.end()) {
        itr->renderOscData(buffSize);
        itr->ampProcessing(buffSize, sampleRate);
        itr++;
    }
}

void PolySynth::mergeWaveData(float* hostBuffer, int buffSize) {
    vItr itr = vecVoice.begin();

    while (itr != vecVoice.end()) {
        for (int n = 0; n < buffSize; n++) {
            hostBuffer[n] += itr->getWaveDataBuffData(n) / maxNumVoice;
        }
        itr++;
    }
}

void synth::doProcessing(PolySynth& sy, AudioSampleBuffer& audio, MidiBuffer& midi, double sampleRate) {
    double fs = sampleRate;
    //midiProcessing...
    MidiBuffer::Iterator mItr(midi);
    MidiMessage m;
    int mPos;

    while (mItr.getNextEvent(m, mPos)) {
        if (m.isNoteOn()) { sy.midiOnProcessing(m, fs); }
        else { sy.midiOffProcessing(m, fs); }
    }

    //audioProcessing...
    float* leftChBuff = audio.getWritePointer(0);
    float* rightChBuff = audio.getWritePointer(1);
    int buffSize = audio.getNumSamples();

    sy.waveRender(buffSize, fs);
    sy.mergeWaveData(leftChBuff, buffSize);

    for (int n = 0; n < buffSize; n++) {
        rightChBuff[n] = leftChBuff[n];
    }
}