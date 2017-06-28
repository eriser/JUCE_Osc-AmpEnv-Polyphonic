//SineOscillator.h===========================================================================
#pragma once

//円周率とsin()関数、midiのあれこれを利用したいので
#include "JuceHeader.h"

class SineOscillator {
public:
	void runOscillator(double frequency, double sampleRate);	//midiOn入力があった時の処理
	void stopOscillator();			//midiOff入力があった時の処理
	virtual double get();			//現在サンプル時間の波形データを取得
	void phaseIncrement();			//1サンプル時間進める

	void changeFrequency(double newVal);	//追加

protected:
	bool _isRunning = false;	//現在、OscにMidiOn状態なのかOff状態なのか
	double _n = 0;				//波形生成を開始してからの経過サンプル時間 (周期化するので整数値以外も取りうる)
	double _f = 440;			//周波数
	double _fs = 48000.0;		//サンプリング周波数
	double _p = 100;			//周期 (サンプル時間)

private:
	//NVIイディオム
	virtual void runOscillatorNVI() {};
	virtual void stopOscillatorNVI() {};
	virtual void phaseIncrementNVI() {};
};