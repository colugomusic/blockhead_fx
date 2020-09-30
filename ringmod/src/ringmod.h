#pragma once

#include <rack++/module/basic_stereo_effect.h>
#include <snd/audio/ringmod/ring_modulator.h>

class RingModulator : public rack::BasicStereoEffect
{
	rack::SmoothParam* param_freq_ = nullptr;
	rack::SmoothParam* param_amount_ = nullptr;
	rack::Trigger* trigger_reset_ = nullptr;

	snd::audio::ringmod::RingModulator<2> ringmod_;

	void on_trigger_fired(const rack::Trigger* t) override;

	ml::DSPVectorArray<2> operator()(const ml::DSPVectorArray<2>& in) override;

	void effect_clear() override {}
public:

	RingModulator();

};