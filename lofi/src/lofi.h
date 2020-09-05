#pragma once

#include <rack++/module/basic_stereo_effect.h>

class Lofi : public rack::BasicStereoEffect
{
	rack::SmoothParam* param_sr_ = nullptr;
	rack::SmoothParam* param_bitrate_ = nullptr;

	int SR_ = 44100;
	float step_ = 1.0f;
	float rate_ = 0.0f;
	float inc_ = 0.0f;
	float phase_ = 0.0f;
	float out_[2] = { 0.0f, 0.0f };

	void on_sample_rate_changed(int new_SR) override;

	ml::DSPVectorArray<2> operator()(const ml::DSPVectorArray<2>& in) override;

	static float calculate_inc(int SR, float rate);

public:

	Lofi();
};