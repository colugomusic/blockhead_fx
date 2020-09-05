#include <algorithm>
#include <rack++/module/channel.h>
#include <rack++/module/smooth_param.h>
#include <rack++/module/trigger.h>
#include "lofi.h"

using namespace snd;
using namespace rack;
using namespace std::placeholders;

Lofi::Lofi()
	: BasicStereoEffect("Lofi")
{
	param_sr_ = add_smooth_param(100.0f, "Sample Rate");

	param_sr_->add_callback([this](float v)
	{
		rate_ = v / 100.0f;
		inc_ = calculate_inc(SR_, rate_);
	});

	param_sr_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_sr_->set_min(0.0f);
	param_sr_->set_max(100.0f);

	param_bitrate_ = add_smooth_param(0.0f, "Bit Crush");

	param_bitrate_->add_callback([this](float v)
	{
		step_ = std::pow(0.5f, std::pow(1.0f - (v / 100.0f), 2.0f) * 16.0f);
	});

	param_bitrate_->set_size_hint(0.75f);
	param_bitrate_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_bitrate_->set_min(0.0f);
	param_bitrate_->set_max(100.0f);

	param_sr_->begin_notify();
	param_bitrate_->begin_notify();
}

float Lofi::calculate_inc(int SR, float rate)
{
	const auto scale = std::tan(std::min(1.57079f, std::pow(std::max(0.0f, rate), 2.0f) * 1.5708f)) * 5000.0f;
	const auto freq = std::min(float(SR), scale);

	return (1.0f / float(SR)) * freq;
}

void Lofi::on_sample_rate_changed(int new_SR)
{
	SR_ = new_SR;

	inc_ = calculate_inc(SR_, rate_);
}

ml::DSPVectorArray<2> Lofi::operator()(const ml::DSPVectorArray<2>& in)
{
	ml::DSPVectorArray<2> out;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		auto ph = phase_ + inc_;
		auto r = int(std::round(ph));

		phase_ = ph - r;

		if (r != 0)
		{
			out_[0] = in.constRow(0)[i];
			out_[1] = in.constRow(1)[i];
		}

		out.row(0)[i] = out_[0];
		out.row(1)[i] = out_[1];
	}

	return ml::intToFloat(ml::truncateFloatToInt((out / step_) + 0.5f)) * step_;
}