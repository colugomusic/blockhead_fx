#pragma once

#include <rack++/module/basic_stereo_effect.h>

class Saturator : public rack::BasicStereoEffect
{
	rack::Param* param_drive_ = nullptr;
	rack::Param* param_ceiling_ = nullptr;

	void on_param_value_changed(const rack::Param* p) override;
	void on_sample_rate_changed() override;

	void process_left(float in, float* out) override;
	void process_right(float in, float* out) override;

public:

	Saturator();
};