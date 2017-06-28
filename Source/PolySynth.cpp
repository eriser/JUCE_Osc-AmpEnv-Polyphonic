#include "PolySynth.h"

using namespace synth;

PolySynth::PolySynth(AudioProcessor& processor)
    :oscParam(processor),
    ampParam(processor),
    //Synthは、OscParameter, ampParameterの参照を受け取るように変更
    vecVoice(maxNumVoice, Synth(oscParam, ampParam))
{
    for (int n = 0; n < maxNumVoice; n++) {
        //VoiceStateに固有のIdを与えてVoice数分コンストラクト
        //初期状態では、すべてのVoiceがOff
        //0 ~ (maxNumVoice - 1)のIdはvecVoice[n]のnに対応する
        lisOffVoiceState.push_back(VoiceState(n));
    }
}

void PolySynth::midiOnProcessing(MidiMessage& m, double sampleRate) {
    int inputNoteNum = m.getNoteNumber();
    int changeVoiceId;  //noteOnにするVoiceId
    lItr itr;   //変更対象となるVoiceStateを指す
    //空のボイスが存在する
    if (!lisOffVoiceState.empty()) {
        //先頭の空ボイスを変更対象に取る
        itr = lisOffVoiceState.begin();
        changeVoiceId = itr->voiceId;
        itr->midiNoteNum = inputNoteNum;
        //変更のあったVoiceStateをlisOnVoiceStateの末尾へ移す
        lisOnVoiceState.push_back(VoiceState(*itr));
        lisOffVoiceState.pop_front();
    }
    //空のボイスが存在しない場合は、On状態にあるVoiceの一番古いものを上書き
    else {
        //push_backで詰めているのでlistの先頭が一番古いVoiceになる
        //後は、先と基本一緒 (コードの重複が気になるが、今回は処理の流れのわかりやすさ優先で)
        itr = lisOnVoiceState.begin();
        changeVoiceId = itr->voiceId;
        itr->midiNoteNum = inputNoteNum;
        lisOnVoiceState.push_back(VoiceState(*itr));
        lisOnVoiceState.pop_front();    //先と違うので注意
    }
    //対応するVoiceをOn状態に
    vecVoice[changeVoiceId].midiProcessing(m, sampleRate);
}

void PolySynth::midiOffProcessing(MidiMessage& m, double sampleRate) {
    int inputNoteNum = m.getNoteNumber();
    int changeVoiceId;  //noteOffにするVoiceId
    lItr itr = lisOnVoiceState.begin();   //変更対象となるVoiceStateを指す

    //midiOffになるべきNoteNumberを保持するVoiceStateを探索
    while (itr != lisOnVoiceState.end()) {
        //変更すべきボイスが見つかったら、そのボイスをlisVoiceOff末尾に移す
        if (itr->midiNoteNum == inputNoteNum) {
            changeVoiceId = itr->voiceId;
            itr->midiNoteNum = -1;
            lisOffVoiceState.push_back(VoiceState(*itr));
            lisOnVoiceState.erase(itr);
            break;
        }
        itr++;
    }
    //対応するVoiceをOff状態に
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