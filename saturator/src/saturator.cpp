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
	param_drive_ = add_smooth_param("Drive");

	param_drive_->add_callback([this](float value) { saturator_.set_drive(value / 100.0f); });

	param_drive_->set_max(100.0f);
	param_drive_->set_format_hint(Rack_ParamFormatHint_Percentage);

	param_gain_ = add_smooth_param("Gain");

	param_gain_->add_callback([this](float value) { gain_af_ = snd::convert::dB2AF(value); });

	param_gain_->set_format_hint(Rack_ParamFormatHint_Decibels);
	param_gain_->set_min(-30.0f);
	param_gain_->set_max(0.0f);
	param_gain_->set(0.0f);
	param_gain_->set_default_value(0.0f);
	param_gain_->set_size_hint(0.75f);

	param_drive_->begin_notify();
	param_gain_->begin_notify();
}

void Saturator::process_left(float in, float* out)
{
	*out = saturator_.process_left(in) * gain_af_;
}

void Saturator::process_right(float in, float* out)
{
	*out = saturator_.process_right(in) * gain_af_;
}