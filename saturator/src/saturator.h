#pragma once

#include <rack++/module/basic_stereo_effect.h>
#include <snd/audio/saturator/moron_saturator_stereo.h>

class Saturator : public rack::BasicStereoEffect
{
	rack::SmoothParam* param_drive_ = nullptr;
	rack::SmoothParam* param_gain_ = nullptr;

	snd::audio::saturator::MoronSaturator_Stereo saturator_;

	float gain_af_ = 1.0f;

	ml::DSPVectorArray<2> operator()(const ml::DSPVectorArray<2>& in) override;

public:

	Saturator();
};