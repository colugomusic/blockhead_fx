#include <algorithm>
#include "1-pole.h"
#include <rack++/module/smooth_param.h>
#include <rack++/module/channel.h>

using namespace rack;
using namespace std::placeholders;

Filter_1Pole::Filter_1Pole()
	: BasicStereoEffect("Filter (1P)")
{
	param_freq_ = add_smooth_param(1000.0f, "Frequency");
	param_freq_->add_callback(std::bind(&snd::audio::filter::Filter_1Pole_Stereo::set_freq, filter_, _1, true));
	param_freq_->set_min(0.08f);
	param_freq_->set_max(16700.0f);
	param_freq_->set_format_hint(Rack_ParamFormatHint_Hertz);

	param_mode_ = add_switch_param({ "LP", "HP" }, "Mode");

	param_mode_->set_min(0);
	param_mode_->set_max(1);

	param_freq_->begin_notify();
	param_mode_->begin_notify();
}

ml::DSPVectorArray<2> Filter_1Pole::operator()(const ml::DSPVectorArray<2>& in)
{
	ml::DSPVectorArray<2> out;

	filter_(in);

	switch (mode_)
	{
		default:
		case Mode::LP:
			return filter_.lp();

		case Mode::HP:
			return filter_.hp();
	}
}

void Filter_1Pole::on_param_value_changed(const Param* p, float new_value)
{
	if (p == param_mode_)
	{
		mode_ = new_value < 1.0 ? Mode::LP : Mode::HP;

		return;
	}
}

void Filter_1Pole::on_sample_rate_changed(int new_SR)
{
	filter_.set_sr(new_SR);
}
