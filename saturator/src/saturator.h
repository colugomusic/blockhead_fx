#pragma once

#include <rack++/module/basic_stereo_effect.h>
#include <snd/audio/saturator/moron_saturator_stereo.h>

class Saturator : public rack::BasicStereoEffect
{
	rack::Param* param_drive_ = nullptr;

	snd::audio::saturator::MoronSaturator_Stereo saturator_;

	void on_param_value_changed(const rack::Param* p) override;

	void process_left(float in, float* out) override;
	void process_right(float in, float* out) override;

public:

	Saturator();
};