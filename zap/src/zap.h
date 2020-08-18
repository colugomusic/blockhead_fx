#pragma once

#include <rack++/module/basic_stereo_effect.h>

class Zap : public rack::BasicStereoEffect
{
	rack::Param* param_spread_ = nullptr;
	rack::Param* param_freq_ = nullptr;
	rack::Param* param_res_ = nullptr;
	rack::Param* param_mix_ = nullptr;

	void on_param_value_changed(const rack::Param* p) override;
	void on_sample_rate_changed() override;

	void process_left(float in, float* out) override;
	void process_right(float in, float* out) override;

public:

	Zap();
};