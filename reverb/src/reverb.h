#pragma once

#include <rack++/module/basic_stereo_effect.h>

#pragma warning(push, 0)
#include <DSP/MLDSPFilters.h>
#include <DSP/MLDSPOps.h>
#pragma warning(pop)

class Reverb : public rack::BasicStereoEffect
{
	rack::Param* param_size_ = nullptr;
	rack::Param* param_decay_ = nullptr;
	rack::SmoothParam* param_mix_ = nullptr;

	ml::LinearGlide glide_size_;
	ml::LinearGlide glide_decay_;

	ml::Allpass<ml::PitchbendableDelay> mAp1, mAp2, mAp3, mAp4;
	ml::Allpass<ml::PitchbendableDelay> mAp5, mAp6, mAp7, mAp8, mAp9, mAp10;
	ml::PitchbendableDelay mDelayL, mDelayR;

	// feedback storage
	ml::DSPVector mvFeedbackL, mvFeedbackR;

	ml::DSPVectorArray<2> operator()(const ml::DSPVectorArray<2>& in) override;

	void on_sample_rate_changed(int new_SR) override;

	void effect_clear() override;

public:

	Reverb();

};