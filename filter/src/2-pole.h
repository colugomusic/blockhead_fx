#pragma once

#include <snd/audio/filter/2-pole.h>
#include <snd/control/smoother.h>
#include <rack++/module/basic_stereo_effect.h>

#pragma warning(push, 0)
#include <DSP/MLDSPGens.h>
#pragma warning(pop)

class Filter_2Pole : public rack::BasicStereoEffect
{
	rack::SmoothParam* param_freq_ = nullptr;
	rack::SmoothParam* param_res_ = nullptr;
	rack::Param* param_mode_ = nullptr;

	enum class Mode { LP, BP, HP } mode_ = Mode::LP;

	snd::audio::filter::Filter_2Pole<2> filter_;

	void on_param_value_changed(const rack::Param* p, float new_value) override;

	ml::DSPVectorArray<2> operator()(const ml::DSPVectorArray<2>& in) override;

	void effect_clear() override {}
public:

	Filter_2Pole();

};