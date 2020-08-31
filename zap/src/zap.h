#pragma once

#include <rack++/module/basic_stereo_effect.h>
#include <snd/audio/filter/2-pole_allpass_array_stereo.h>

class Zap : public rack::BasicStereoEffect
{
	static constexpr float MIN_FREQ = 130.0f;
	static constexpr float MAX_FREQ = 4000.0f;

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

	ml::DSPVectorArray<2> operator()(const ml::DSPVectorArray<2>& in) override;

public:

	Zap();

};