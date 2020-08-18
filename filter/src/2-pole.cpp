#include "2-pole.h"
#include <rack++/module/channel.h>
#include <rack++/module/param.h>
#include <algorithm>

using namespace rack;

Filter_2Pole::Filter_2Pole()
	: BasicStereoEffect("Filter (2P)")
{
	param_freq_ = add_param();

	param_freq_->name = "Frequency";
	param_freq_->set(600.0f);
	param_freq_->set_format_hint(Rack_ParamFormatHint_Hertz);
	param_freq_->set_min(0.08f);
	param_freq_->set_max(16700.0f);

	param_res_ = add_param();

	param_res_->name = "Resonance";
	param_res_->set(0.5f);
	param_res_->set_size_hint(0.75f);

	param_mode_ = add_param();

	param_mode_->name = "Mode";
	param_mode_->set_format_hint(Rack_ParamFormatHint_Switch);
	param_mode_->set_min(0);
	param_mode_->set_max(2);

	param_freq_->begin_notify();
	param_res_->begin_notify();
	param_mode_->begin_notify();
}

void Filter_2Pole::on_param_value_changed(const Param* p)
{
	if (p == param_freq_)
	{
		const auto hz = param_freq_->get();

		filter_.set_freq(hz);
	}

	if (p == param_res_)
	{
		const auto res = param_res_->get();

		filter_.set_res(res);
	}

	if (p == param_mode_)
	{
		const auto mode_value = param_mode_->get();

		mode_ = mode_value < 1.0 ? Mode::LP : mode_value < 2.0 ? Mode::BP : Mode::HP;
	}
}

void Filter_2Pole::on_sample_rate_changed()
{
	filter_.set_sr(float(sample_rate_));
}

void Filter_2Pole::process_left(float in, float* out)
{
	filter_.process_left(in);

	switch (mode_)
	{
		case Mode::LP:
			*out = filter_.lp_L();
			break;

		case Mode::BP:
			*out = filter_.bp_L();
			break;

		case Mode::HP:
			*out = filter_.hp_L();
			break;
	}
}

void Filter_2Pole::process_right(float in, float* out)
{
	filter_.process_right(in);

	switch (mode_)
	{
		case Mode::LP:
			*out = filter_.lp_R();
			break;

		case Mode::BP:
			*out = filter_.bp_R();
			break;

		case Mode::HP:
			*out = filter_.hp_R();
			break;
	}
}