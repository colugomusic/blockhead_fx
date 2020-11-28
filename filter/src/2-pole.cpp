#include "2-pole.h"
#include <rack++/module/channel.h>
#include <rack++/module/smooth_param.h>
#include <rack++/module/trigger.h>
#include <algorithm>

using namespace rack;
using namespace std::placeholders;

Filter_2Pole::Filter_2Pole()
	: BasicStereoEffect("Filter (2P)")
{
	param_freq_ = add_smooth_param(1000.0f, "Frequency");
	param_res_ = add_smooth_param(0.0f, "Resonance");

	param_res_->set_transform([](const ml::DSPVector& v) { return v / 100.0f; });

	param_freq_->set_format_hint(Rack_ParamFormatHint_Hertz);
	param_freq_->set_min(0.08f);
	param_freq_->set_max(16700.0f);

	param_res_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_res_->set_min(0.0f);
	param_res_->set_max(100.0f);
	param_res_->set_size_hint(0.75f);

	param_mode_ = add_switch_param({ "LP", "BP", "HP" }, "Mode");

	param_mode_->set_min(0);
	param_mode_->set_max(2);

	param_freq_->begin_notify();
	param_res_->begin_notify();
	param_mode_->begin_notify();
}

void Filter_2Pole::on_param_value_changed(const Param* p, float new_value)
{
	if (p == param_mode_)
	{
		mode_ = new_value < 1.0 ? Mode::LP : new_value < 2.0 ? Mode::BP : Mode::HP;
	}
}

ml::DSPVectorArray<2> Filter_2Pole::operator()(const ml::DSPVectorArray<2>& in)
{
	ml::DSPVectorArray<2> out;

	filter_(in, sample_rate_, ml::repeatRows<2>((*param_freq_)()), ml::repeatRows<2>((*param_res_)()));

	switch (mode_)
	{
		default:
		case Mode::LP:
			return filter_.lp();

		case Mode::BP:
			return filter_.bp();

		case Mode::HP:
			return filter_.hp();
	}
}