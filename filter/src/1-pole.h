#pragma once

#include <snd/audio/filter/1-pole.h>
#include <rack++/module/basic_stereo_effect.h>

class Filter_1Pole : public rack::BasicStereoEffect
{
	rack::SmoothParam* param_freq_ = nullptr;
	rack::Param* param_mode_ = nullptr;

	enum class Mode { LP, HP } mode_ = Mode::LP;

	snd::audio::filter::Filter_1Pole<2> filter_;

	void on_param_value_changed(const rack::Param* p, float new_value) override;

	ml::DSPVectorArray<2> operator()(const ml::DSPVectorArray<2>& in) override;

public:

	Filter_1Pole();
};