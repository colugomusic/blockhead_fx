#pragma once

#include <rack++/module/basic_stereo_effect.h>

class Zap : public rack::BasicStereoEffect
{
	rack::SmoothParam* param_spread_ = nullptr;
	rack::SmoothParam* param_freq_ = nullptr;
	rack::SmoothParam* param_res_ = nullptr;
	rack::SmoothParam* param_mix_ = nullptr;

	void on_sample_rate_changed(int new_SR) override;

	void process_left(float in, float* out) override;
	void process_right(float in, float* out) override;

public:

	Zap();
};