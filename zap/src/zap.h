#pragma once

#include <rack++/module/basic_stereo_effect.h>
#include <snd/audio/filter/2-pole_allpass_array.h>

class Zap : public rack::BasicStereoEffect
{
	static constexpr float MIN_FREQ = 130.0f;
	static constexpr float MAX_FREQ = 4000.0f;

	rack::SmoothParam* param_spread_ = nullptr;
	rack::SmoothParam* param_freq_ = nullptr;
	rack::SmoothParam* param_res_ = nullptr;
	rack::SmoothParam* param_mix_ = nullptr;

	snd::audio::filter::Filter_2Pole_AllpassArray<2, 32> filter_;

	ml::DSPVectorArray<2> operator()(const ml::DSPVectorArray<2>& in) override;

	void effect_clear() override;

public:

	Zap();


};