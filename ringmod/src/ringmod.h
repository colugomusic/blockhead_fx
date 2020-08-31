#pragma once

#include <rack++/module/basic_stereo_effect.h>
#include <snd/audio/ringmod/ring_modulator_stereo.h>

class RingModulator : public rack::BasicStereoEffect
{
	rack::SmoothParam* param_freq_ = nullptr;
	rack::SmoothParam* param_amount_ = nullptr;
	rack::Trigger* trigger_reset_ = nullptr;

	snd::audio::ringmod::RingModulator_Stereo ringmod_;

	void on_sample_rate_changed(int new_SR) override;
	void on_trigger_fired(const rack::Trigger* t) override;

	ml::DSPVectorArray<2> operator()(const ml::DSPVectorArray<2>& in) override;

public:

	RingModulator();
};