#include <rack++/module/smooth_param.h>
#include <rack++/module/channel.h>
#include "saturator.h"
#include <snd/convert.h>

using namespace rack;
using namespace snd;
using namespace std::placeholders;

Saturator::Saturator()
	: BasicStereoEffect("Saturator")
{
	param_drive_ = add_smooth_param(0.0f, "Drive");

	param_drive_->set_transform([](const ml::DSPVector& v) { return v / 100.0f; });

	param_drive_->set_max(100.0f);
	param_drive_->set_format_hint(Rack_ParamFormatHint_Percentage);

	param_gain_ = add_smooth_param(0.0f, "Gain");

	param_gain_->set_transform([](const ml::DSPVector& v) { return snd::convert::dB2AF(v); });

	param_gain_->set_format_hint(Rack_ParamFormatHint_Decibels);
	param_gain_->set_min(-30.0f);
	param_gain_->set_max(0.0f);
	param_gain_->set_size_hint(0.75f);

	param_drive_->begin_notify();
	param_gain_->begin_notify();
}

ml::DSPVectorArray<2> Saturator::operator()(const ml::DSPVectorArray<2>& in)
{
	return saturator_(in, ml::repeat<2>((*param_drive_)())) * ml::repeat<2>((*param_gain_)());
}