#include "2-pole_peak.h"
#include <rack++/module/channel.h>
#include <rack++/module/smooth_param.h>
#include <rack++/module/trigger.h>
#include <algorithm>

using namespace rack;
using namespace std::placeholders;

Filter_2Pole_Peak::Filter_2Pole_Peak()
	: BasicStereoEffect("Peak (2P)")
{
	param_freq_ = add_smooth_param(1000.0f, "Frequency");
	param_res_ = add_smooth_param(0.0f, "Resonance");

	param_freq_->add_callback(std::bind(&snd::audio::filter::Filter_2Pole_Stereo::set_freq, &filter_, _1, true));

	param_freq_->set_format_hint(Rack_ParamFormatHint_Hertz);
	param_freq_->set_min(0.08f);
	param_freq_->set_max(16700.0f);

	param_res_->add_callback([this](float value) { filter_.set_res(value / 100.0f); });

	param_res_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_res_->set_min(0.0f);
	param_res_->set_max(100.0f);
	param_res_->set_size_hint(0.75f);

	param_freq_->begin_notify();
	param_res_->begin_notify();
}

void Filter_2Pole_Peak::on_sample_rate_changed(int new_SR)
{
	filter_.set_sr(new_SR);
}

ml::DSPVectorArray<2> Filter_2Pole_Peak::operator()(const ml::DSPVectorArray<2>& in)
{
	ml::DSPVectorArray<2> out;

	filter_(in);

	return filter_.peak();
}