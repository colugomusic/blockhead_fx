#include <algorithm>
#include "1-pole.h"
#include <rack++/module/param.h>
#include <rack++/module/channel.h>

using namespace rack;

Filter_1Pole::Filter_1Pole()
	: BasicStereoEffect("Filter (1P)")
{
	param_freq_ = add_param();

	param_freq_->name = "Frequency";
	param_freq_->set(600.0f);
	param_freq_->set_min(0.08f);
	param_freq_->set_max(16700.0f);

	param_mode_ = add_param();

	param_mode_->name = "Mode";
	param_mode_->set_format_hint(Rack_ParamFormatHint_Switch);
	param_mode_->set_min(0);
	param_mode_->set_max(1);

	param_freq_->begin_notify();
	param_mode_->begin_notify();
}

void Filter_1Pole::on_param_value_changed(const Param* p)
{
	if (p == param_freq_)
	{
		const auto hz = param_freq_->get();

		filter_.set_freq(hz);

		return;
	}

	if (p == param_mode_)
	{
		const auto mode_value = param_mode_->get();

		mode_ = mode_value < 1.0 ? Mode::LP : Mode::HP;

		return;
	}
}

void Filter_1Pole::on_sample_rate_changed()
{
	filter_.set_sr(float(sample_rate_));
}

void Filter_1Pole::process_left(float in, float* out)
{
	filter_.process_left(in);

	switch (mode_)
	{
		case Mode::LP:
			*out = filter_.lp_L();
			break;

		case Mode::HP:
			*out = filter_.hp_L();
			break;
	}
}

void Filter_1Pole::process_right(float in, float* out)
{
	filter_.process_right(in);

	switch (mode_)
	{
		case Mode::LP:
			*out = filter_.lp_R();
			break;

		case Mode::HP:
			*out = filter_.hp_R();
			break;
	}
}
