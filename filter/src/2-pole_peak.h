#pragma once

#include <snd/audio/filter/2-pole.h>
#include <snd/control/smoother.h>
#include <rack++/module/basic_stereo_effect.h>

class Filter_2Pole_Peak : public rack::BasicStereoEffect
{
	rack::SmoothParam* param_freq_ = nullptr;
	rack::SmoothParam* param_res_ = nullptr;
	rack::Param* param_mode_ = nullptr;

	snd::audio::filter::Filter_2Pole<2> filter_;

	ml::DSPVectorArray<2> operator()(const ml::DSPVectorArray<2>& in) override;

	void effect_clear() override {}
public:

	Filter_2Pole_Peak();

};