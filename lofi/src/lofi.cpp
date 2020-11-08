#include <algorithm>
#include <rack++/module/channel.h>
#include <rack++/module/smooth_param.h>
#include <rack++/module/trigger.h>
#include <snd/misc.h>
#include "lofi.h"

using namespace snd;
using namespace rack;
using namespace std::placeholders;

Lofi::Lofi()
	: BasicStereoEffect("Lofi")
{
	param_sr_ = add_smooth_param(100.0f, "Sample Rate");

	param_sr_->set_transform([this](const ml::DSPVector& v)
	{
		return v / 100.0f;
	});

	param_sr_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_sr_->set_min(0.0f);
	param_sr_->set_max(100.0f);

	param_bitrate_ = add_smooth_param(0.0f, "Bit Crush");

	param_bitrate_->set_transform([](const ml::DSPVector& v)
	{
		ml::DSPVector out;

		for (int i = 0; i < kFloatsPerDSPVector; i++)
		{
			const auto x = (v[i] / 100.0f);

			out[i] = x * std::pow(0.1f, std::pow(1.0f - x, 2.0f)) * 0.9f;
		}

		return out;
	});

	param_bitrate_->set_size_hint(0.75f);
	param_bitrate_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_bitrate_->set_min(0.0f);
	param_bitrate_->set_max(100.0f);

	param_sr_->begin_notify();
	param_bitrate_->begin_notify();
}

static ml::DSPVector tan(const ml::DSPVector& x)
{
	ml::DSPVector out;

	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		out[i] = std::tan(x[i]);
	}

	return out;
}

ml::DSPVector Lofi::calculate_inc(int SR, const ml::DSPVector& rate)
{
	const auto scale = tan(ml::min(ml::DSPVector(1.57079f), ml::pow(ml::max(ml::DSPVector(0.0f), rate), ml::DSPVector(2.0f)) * 1.5708f)) * 5000.0f;
	const auto freq = ml::min(ml::DSPVector(float(SR)), scale);

	return (1.0f / float(SR)) * freq;
}

ml::DSPVectorArray<2> Lofi::operator()(const ml::DSPVectorArray<2>& in)
{
	ml::DSPVectorArray<2> out;

	const auto raw_rate = (*param_sr_)();
	const auto rate = ml::lerp(ml::DSPVector(0.1f), ml::DSPVector(1.0f), raw_rate);
	const auto inc = calculate_inc(sample_rate_, rate);
	
	for (int i = 0; i < kFloatsPerDSPVector; i++)
	{
		auto ph = phase_ + inc[i];
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

	auto step = ml::repeat<2>((*param_bitrate_)());

	return ml::select(ml::intToFloat(ml::truncateFloatToInt((out / step) + 0.5f)) * step, out, ml::greaterThan(step, ml::DSPVectorArray<2>(0.0f)));
}