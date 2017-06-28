#pragma once

#include "AmpParameters.h"

class Amplifier {
public:
	//MidiOnになった時の処理
	void setGateOnState(AmpParameters&, double sampleRate);
	//MidiOffになった時の処理
	void setGateOffState(AmpParameters&, double sampleRate);
	//アンプEvnの状態に応じて、渡されたオーディオのボリュームを絞る
	void applyAmpEvnProcessing(float& audioSignal);
	//アンプEvn内の時間を進める・状態の切り替え判定
	void setNextState(AmpParameters&, double sampleRate);
	bool isOffState();

private:
	//AmpEvnの状態
	enum EvnState {
		Attack = 0, Decay, Sustain, Release, Off
	};

	int _state = Off;		//現在サンプル時間のAmpEvnの状態
	int _n = 0;				//現在の状態に移ってからの経過サンプル時間
	int _duration = 1;		//次の状態に移るまでのサンプル時間
	double _gain = 0.0;		//どの程度音量を絞るか
	double _sloop = 0.0;	//Evnカーブの傾き
};