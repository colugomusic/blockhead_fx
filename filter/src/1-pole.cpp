#include <algorithm>
#include "1-pole.h"
#include <rack++/module/smooth_param.h>
#include <rack++/module/channel.h>

using namespace rack;
using namespace std::placeholders;

Filter_1Pole::Filter_1Pole()
	: BasicStereoEffect("Filter (1P)")
{
	param_freq_ = add_smooth_param("Frequency");
	param_freq_->add_callback(std::bind(&snd::audio::filter::Filter_1Pole_Stereo::set_freq, filter_, _1, true));
	param_freq_->set(600.0f);
	param_freq_->set_min(0.08f);
	param_freq_->set_max(16700.0f);

	param_mode_ = add_switch_param({ "LP", "HP" }, "Mode");

	param_mode_->set_min(0);
	param_mode_->set_max(1);

	param_freq_->begin_notify();
	param_mode_->begin_notify();
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
	filter_.set_sr(float(new_SR));
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
