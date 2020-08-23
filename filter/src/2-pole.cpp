#include "2-pole.h"
#include <rack++/module/channel.h>
#include <rack++/module/smooth_param.h>
#include <algorithm>

using namespace rack;
using namespace std::placeholders;

Filter_2Pole::Filter_2Pole()
	: BasicStereoEffect("Filter (2P)")
	//, smoother_freq_(sample_rate_, 0.02f, 1000.0f, std::bind(&snd::audio::filter::Filter_2Pole_Stereo::set_freq, &filter_, std::placeholders::_1, true))
	//, smoother_res_(sample_rate_, 0.02f, 1000.0f, std::bind(&snd::audio::filter::Filter_2Pole_Stereo::set_res, &filter_, std::placeholders::_1, true))
{
	param_freq_ = add_smooth_param(1000.0f, "Frequency");

	param_freq_->add_callback(std::bind(&snd::audio::filter::Filter_2Pole_Stereo::set_freq, &filter_, _1, true));

	param_freq_->set_format_hint(Rack_ParamFormatHint_Hertz);
	param_freq_->set_min(0.08f);
	param_freq_->set_max(16700.0f);

	param_res_ = add_smooth_param(0.0f, "Resonance");

	param_res_->add_callback([this](float value) { filter_.set_res(value / 100.0f); });

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

void Filter_2Pole::copy(const Filter_2Pole& rhs)
{
	Unit::copy(rhs);

	res_ = rhs.res_;
	mode_ = rhs.mode_;

	filter_ = rhs.filter_;
}

void Filter_2Pole::reset()
{
	Unit::reset();

	res_ = 0.0f;
	mode_ = Mode::LP;

	filter_ = snd::audio::filter::Filter_2Pole_Stereo();
}

void Filter_2Pole::on_param_value_changed(const Param* p, float new_value)
{
	if (p == param_mode_)
	{
		mode_ = new_value < 1.0 ? Mode::LP : new_value < 2.0 ? Mode::BP : Mode::HP;
	}
}

void Filter_2Pole::on_sample_rate_changed(int new_SR)
{
	filter_.set_sr(new_SR);
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