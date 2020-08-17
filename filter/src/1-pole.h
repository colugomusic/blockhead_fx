#pragma once

#include <snd/audio/filter/1-pole_stereo.h>
#include <rack++/module/basic_stereo_effect.h>

class Filter_1Pole : public rack::BasicStereoEffect
{
	rack::Param* param_freq_ = nullptr;
	rack::Param* param_mode_ = nullptr;

	enum class Mode { LP, HP } mode_;

	snd::audio::filter::Filter_1Pole_Stereo filter_;

	void on_param_value_changed(const rack::Param* p) override;
	void on_sample_rate_changed() override;

	void process_left(float in, float* out) override;
	void process_right(float in, float* out) override;

public:

	Filter_1Pole();
};