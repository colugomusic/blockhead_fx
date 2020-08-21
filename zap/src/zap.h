#pragma once

#include <rack++/module/basic_stereo_effect.h>
#include <snd/audio/filter/2-pole_allpass_array_stereo.h>

class Zap : public rack::BasicStereoEffect
{
	rack::SmoothParam* param_spread_ = nullptr;
	rack::SmoothParam* param_freq_ = nullptr;
	rack::SmoothParam* param_res_ = nullptr;
	rack::SmoothParam* param_mix_ = nullptr;

	float freq_ = 400.0f;
	float spread_ = 0.0f;
	float mix_ = 1.0f;

	snd::audio::filter::Filter_2Pole_AllpassArray_Stereo<32> filter_;

	void on_freq_changed(float v);
	void on_spread_changed(float v);
	void on_sample_rate_changed(int new_SR) override;

	void process_left(float in, float* out) override;
	void process_right(float in, float* out) override;

public:

	Zap();
};