#include <algorithm>
#include <rack++/module/channel.h>
#include <rack++/module/smooth_param.h>
#include <rack++/module/trigger.h>
#include "Reverb.h"

#pragma warning(push, 0)
#include <DSP/MLDSPProjections.h>
#pragma warning(pop)

using namespace snd;
using namespace rack;
using namespace std::placeholders;

Reverb::Reverb()
	: BasicStereoEffect("Spring Reverb")
{
	glide_size_.setGlideTimeInSamples(0.1f * sample_rate_);
	glide_decay_.setGlideTimeInSamples(0.1f * sample_rate_);

	// set allpass filter coefficients
	mAp1.mGain = 0.75f;
	mAp2.mGain = 0.70f;
	mAp3.mGain = 0.625f;
	mAp4.mGain = 0.625f;
	mAp5.mGain = mAp6.mGain = 0.7f;
	mAp7.mGain = mAp8.mGain = 0.6f;
	mAp9.mGain = mAp10.mGain = 0.5f;

	// allocate delay memory
	mAp1.setMaxDelayInSamples(500.f);
	mAp2.setMaxDelayInSamples(500.f);
	mAp3.setMaxDelayInSamples(1000.f);
	mAp4.setMaxDelayInSamples(1000.f);
	mAp5.setMaxDelayInSamples(2600.f);
	mAp6.setMaxDelayInSamples(2600.f);
	mAp7.setMaxDelayInSamples(8000.f);
	mAp8.setMaxDelayInSamples(8000.f);
	mAp9.setMaxDelayInSamples(10000.f);
	mAp10.setMaxDelayInSamples(10000.f);
	mDelayL.setMaxDelayInSamples(3500.f);
	mDelayR.setMaxDelayInSamples(3500.f);

	param_size_ = add_param(50.0f, "Size");

	param_size_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_size_->set_min(0.0f);
	param_size_->set_max(100.0f);

	param_decay_ = add_param(50.0f, "Decay");

	param_decay_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_decay_->set_min(0.0f);
	param_decay_->set_max(100.0f);

	param_mix_ = add_smooth_param(50.0f, "Mix");

	param_mix_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_mix_->set_min(0.0f);
	param_mix_->set_max(100.0f);
	param_mix_->set_size_hint(0.75f);

	param_mix_->set_transform([](const ml::DSPVector& v) { return v / 100.0f; });

	param_size_->begin_notify();
	param_decay_->begin_notify();
}

void Reverb::effect_clear()
{
	mAp1.clear();
	mAp2.clear();
	mAp3.clear();
	mAp4.clear();
	mAp5.clear();
	mAp6.clear();
	mAp7.clear();
	mAp8.clear();
	mAp9.clear();
	mAp10.clear();
	mDelayL.clear();
	mDelayR.clear();
	mvFeedbackL = ml::DSPVector();
	mvFeedbackR = ml::DSPVector();
}

void Reverb::on_sample_rate_changed(int new_SR)
{
	glide_size_.setGlideTimeInSamples(0.1f * new_SR);
	glide_decay_.setGlideTimeInSamples(0.1f * new_SR);
}

// log projection for decay parameter
constexpr float kDecayLo = 0.01f, kDecayHi = 20.f;
ml::Projection unityToDecay(ml::projections::intervalMap({ 0, 1 }, { kDecayLo, kDecayHi }, ml::projections::log({ kDecayLo, kDecayHi })));

ml::DSPVectorArray<2> Reverb::operator()(const ml::DSPVectorArray<2>& in)
{
	const float sr = float(sample_rate_);
	const float RT60const = 0.001f;

	float sizeU = ml::lerp(0.01f, 1.0f, param_size_->get() / 100.0f);
	float decayU = param_decay_->get() / 100.0f;

	// generate delay and feedback scalars
	float decayTime = unityToDecay(decayU);
	float decayIterations = decayTime / (sizeU * 0.5f);
	float feedback = (decayU < 1.0f) ? powf(RT60const, 1.0f / decayIterations) : 1.0f;

	if (is_first_buffer())
	{
		glide_size_.setValue(sizeU * 2.0f);
		glide_decay_.setValue(feedback);
	}

	// generate smoothed delay time and feedback gain vectors
	ml::DSPVector vSmoothDelay = glide_size_(sizeU * 2.0f);
	ml::DSPVector vSmoothFeedback = glide_decay_(feedback);

	// get the minimum possible delay in samples, which is the length of a DSPVector.
	ml::DSPVector vMin(kFloatsPerDSPVector);

	// get smoothed allpass times in samples
	ml::DSPVector delayParamInSamples = sr * vSmoothDelay;
	ml::DSPVector vt1 = max(0.00476f * delayParamInSamples, vMin);
	ml::DSPVector vt2 = max(0.00358f * delayParamInSamples, vMin);
	ml::DSPVector vt3 = max(0.00973f * delayParamInSamples, vMin);
	ml::DSPVector vt4 = max(0.00830f * delayParamInSamples, vMin);
	ml::DSPVector vt5 = max(0.029f * delayParamInSamples, vMin);
	ml::DSPVector vt6 = max(0.021f * delayParamInSamples, vMin);
	ml::DSPVector vt7 = max(0.078f * delayParamInSamples, vMin);
	ml::DSPVector vt8 = max(0.090f * delayParamInSamples, vMin);
	ml::DSPVector vt9 = max(0.111f * delayParamInSamples, vMin);
	ml::DSPVector vt10 = max(0.096f * delayParamInSamples, vMin);

	// sum stereo inputs and diffuse with four allpass filters in series
	ml::DSPVector monoInput = (in.constRow(0) + in.constRow(1));
	ml::DSPVector diffusedInput = mAp4(mAp3(mAp2(mAp1(monoInput, vt1), vt2), vt3), vt4);

	// get delay times in samples, subtracting the constant delay of one DSPVector and clamping to zero
	ml::DSPVector vDelayTimeL = max(0.0313f * delayParamInSamples - vMin, ml::DSPVector(0.f));
	ml::DSPVector vDelayTimeR = max(0.0371f * delayParamInSamples - vMin, ml::DSPVector(0.f));

	// sum diffused input with feedback, and apply late diffusion of two more allpass filters to each channel
	ml::DSPVector vTapL = mAp7(mAp5(diffusedInput + mDelayL(mvFeedbackL, vDelayTimeL), vt5), vt7);
	ml::DSPVector vTapR = mAp8(mAp6(diffusedInput + mDelayR(mvFeedbackR, vDelayTimeR), vt6), vt8);

	// apply final allpass filter and gain, and store the feedback
	mvFeedbackR = mAp9(vTapL, vt9) * vSmoothFeedback;
	mvFeedbackL = mAp10(vTapR, vt10) * vSmoothFeedback;

	// append the left and right taps and return the stereo output
	return ml::lerp(in, ml::concatRows(vTapL, vTapR), ml::repeatRows<2>((*param_mix_)()));
}