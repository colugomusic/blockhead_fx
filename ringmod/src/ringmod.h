#pragma once

#include <rack++/module/basic_stereo_effect.h>
#include <snd/audio/ringmod/ring_modulator_stereo.h>

class RingModulator : public rack::BasicStereoEffect
{
	rack::Param* param_freq_ = nullptr;
	rack::Param* param_amount_ = nullptr;
	rack::Trigger* trigger_reset_ = nullptr;

	snd::audio::ringmod::RingModulator_Stereo ringmod_;

	void on_param_value_changed(const rack::Param* p) override;
	void on_sample_rate_changed() override;
	void on_trigger_fired(const rack::Trigger* t) override;

	void process_left(float in, float* out) override;
	void process_right(float in, float* out) override;

public:

	RingModulator();
};