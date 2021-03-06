#pragma once

#include <rack++/module/basic_stereo_effect.h>

#pragma warning(push, 0)
#include <DSP/MLDSPFilters.h>
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

class Lofi : public rack::BasicStereoEffect
{
	rack::SmoothParam* param_sr_ = nullptr;
	rack::SmoothParam* param_bitrate_ = nullptr;

	float phase_ = 0.0f;
	float out_[2] = { 0.0f, 0.0f };

	ml::DSPVectorArray<2> operator()(const ml::DSPVectorArray<2>& in) override;

	static ml::DSPVector calculate_inc(int SR, const ml::DSPVector& rate);

	void effect_clear() override {}
public:

	Lofi();

};
