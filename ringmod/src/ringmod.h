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

	void process_left(float in, float* out) override;
	void process_right(float in, float* out) override;

public:

	RingModulator();

	void copy(const RingModulator& rhs);
	void reset() override;
};