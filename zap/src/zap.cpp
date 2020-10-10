#include <rack++/module/smooth_param.h>
#include <rack++/module/channel.h>
#include <snd/misc.h>
#include "zap.h"

using namespace rack;
using namespace std::placeholders;

Zap::Zap()
	: BasicStereoEffect("Zap")
{
	param_spread_ = add_smooth_param(0.0f, "Spread");
	param_spread_->set_transform([](const ml::DSPVector& v)
	{
		const auto x = v / 100.0f;
		
		return x * x * ml::signBit(x);
	});

	param_spread_->set_size_hint(0.75);
	param_spread_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_spread_->set_min(-100.0f);
	param_spread_->set_max(100.0f);

	param_freq_ = add_smooth_param(1000.0f, "Frequency");
	param_freq_->set_format_hint(Rack_ParamFormatHint_Hertz);
	param_freq_->set_min(MIN_FREQ);
	param_freq_->set_max(MAX_FREQ);

	param_res_ = add_smooth_param(50.0f, "Resonance");
	param_res_->set_transform([](const ml::DSPVector& v) { return v / 100.0f; });
	param_res_->set_size_hint(0.75);
	param_res_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_res_->set_min(0.0f);
	param_res_->set_max(100.0f);

	param_mix_ = add_smooth_param(100.0f, "Mix");
	param_mix_->set_transform([](const ml::DSPVector& v) { return v / 100.0f; });
	param_mix_->set_size_hint(0.5);
	param_mix_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_mix_->set_min(0.0f);
	param_mix_->set_max(100.0f);

	param_spread_->begin_notify();
	param_freq_->begin_notify();
	param_res_->begin_notify();
	param_mix_->begin_notify();
}

ml::DSPVectorArray<2> Zap::operator()(const ml::DSPVectorArray<2>& in)
{
	ml::DSPVectorArray<2> out;

	const auto& spread = (*param_spread_)();
 	const auto& base_freq = (*param_freq_)();
	const auto& res = (*param_res_)();
	const auto& mix = (*param_mix_)();

	const auto offset = spread * 1000.0f;

	const ml::DSPVector min_freq(MIN_FREQ);
	const ml::DSPVector max_freq(MAX_FREQ);

	const auto freq_L = ml::clamp(base_freq - offset, min_freq, max_freq);
	const auto freq_R = ml::clamp(base_freq + offset, min_freq, max_freq);

	const auto freq = ml::append(freq_L, freq_R);

 	out = filter_(in, sample_rate_, freq, ml::repeat<2>(res));

	return ml::lerp(in, out, ml::repeat<2>(mix));
}

void Zap::effect_clear()
{
	filter_.clear();
}
