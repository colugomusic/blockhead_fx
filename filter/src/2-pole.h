#pragma once

#include <snd/audio/filter/2-pole_stereo.h>
#include <rack++/module/basic_stereo_effect.h>

class Filter_2Pole : public rack::BasicStereoEffect
{
	rack::Param* param_freq_ = nullptr;
	rack::Param* param_res_ = nullptr;
	rack::Param* param_mode_ = nullptr;

	enum class Mode { LP, BP, HP } mode_;

	snd::audio::filter::Filter_2Pole_Stereo filter_;

	void on_param_value_changed(const rack::Param* p) override;
	void on_sample_rate_changed() override;

	void process_left(float in, float* out) override;
	void process_right(float in, float* out) override;

public:

	Filter_2Pole();
};