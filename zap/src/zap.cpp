#include <rack++/module/smooth_param.h>
#include <rack++/module/channel.h>
#include <snd/misc.h>
#include "zap.h"

using namespace rack;
using namespace std::placeholders;

Zap::Zap()
	: BasicStereoEffect("Zap")
{
	param_spread_ = add_smooth_param("Spread");
	param_spread_->add_callback(std::bind(&Zap::on_spread_changed, this, _1));
	param_spread_->set_size_hint(0.75);
	param_spread_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_spread_->set_min(-100.0f);
	param_spread_->set_max(100.0f);

	param_freq_ = add_smooth_param("Frequency");
	param_freq_->add_callback(std::bind(&Zap::on_freq_changed, this, _1));
	param_freq_->set(400.0f);
	param_freq_->set_default_value(400.0f);
	param_freq_->set_format_hint(Rack_ParamFormatHint_Hertz);
	param_freq_->set_min(MIN_FREQ);
	param_freq_->set_max(MAX_FREQ);

	param_res_ = add_smooth_param("Resonance");
	param_res_->add_callback([this](float v) { filter_.set_res(v / 100.0f); });
	param_res_->set_size_hint(0.75);
	param_res_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_res_->set_min(0.0f);
	param_res_->set_max(100.0f);

	param_mix_ = add_smooth_param("Mix");
	param_mix_->add_callback([this](float v) { mix_ = v / 100.0f; });
	param_mix_->set_size_hint(0.5);
	param_mix_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_mix_->set_min(0.0f);
	param_mix_->set_max(100.0f);
	param_mix_->set(100.0f);
	param_mix_->set_default_value(100.0f);

	param_spread_->begin_notify();
	param_freq_->begin_notify();
	param_res_->begin_notify();
	param_mix_->begin_notify();
}

void Zap::on_freq_changed(float v)
{
	freq_ = v;

	const auto offset = spread_ * (std::signbit(spread_) ? 1 : -1) * 1000.0f;

	filter_.set_freq_L(std::max(MIN_FREQ, freq_ - offset));
	filter_.set_freq_R(std::min(MAX_FREQ, freq_ + offset));
}

void Zap::on_spread_changed(float v)
{
	spread_ = std::pow(v / 100.0f, 2.f);

	const auto offset = spread_ * (std::signbit(spread_) ? 1 : -1) * 1000.0f;

	filter_.set_freq_L(std::max(MIN_FREQ, freq_ - offset));
	filter_.set_freq_R(std::min(MAX_FREQ, freq_ + offset));
}

void Zap::on_sample_rate_changed(int new_SR)
{
	filter_.set_sr(new_SR);
}

void Zap::process_left(float in, float* out)
{
	*out = snd::lerp(in, filter_.process_L(in), mix_);
}

void Zap::process_right(float in, float* out)
{
	*out = snd::lerp(in, filter_.process_R(in), mix_);
}