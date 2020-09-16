#pragma once

#include <rack++/module/basic_stereo_effect.h>
#include <snd/audio/saturator/moron_saturator.h>

class Saturator : public rack::BasicStereoEffect
{
	rack::SmoothParam* param_drive_ = nullptr;
	rack::SmoothParam* param_gain_ = nullptr;

	snd::audio::saturator::MoronSaturator<2> saturator_;

	ml::DSPVectorArray<2> operator()(const ml::DSPVectorArray<2>& in) override;

public:

	Saturator();
};