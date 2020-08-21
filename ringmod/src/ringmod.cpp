#include <algorithm>
#include <rack++/module/channel.h>
#include <rack++/module/smooth_param.h>
#include <rack++/module/trigger.h>
#include "ringmod.h"

using namespace snd;
using namespace rack;
using namespace std::placeholders;

RingModulator::RingModulator()
	: BasicStereoEffect("Ring Modulator")
{
	param_freq_ = add_smooth_param("Frequency");

	param_freq_->add_callback(std::bind(&audio::ringmod::RingModulator_Stereo::set_freq, &ringmod_, _1, true));

	param_freq_->set_format_hint(Rack_ParamFormatHint_Hertz);
	param_freq_->set(600.0f);
	param_freq_->set_default_value(600.0f);
	param_freq_->set_min(0.08f);
	param_freq_->set_max(16700.0f);

	param_amount_ = add_smooth_param("Amount");

	param_amount_->add_callback(std::bind(&audio::ringmod::RingModulator_Stereo::set_amount, &ringmod_, _1));

	param_amount_->set_size_hint(0.75f);

	trigger_reset_ = add_trigger("Reset");

	param_freq_->begin_notify();
	param_amount_->begin_notify();
}

void RingModulator::on_sample_rate_changed(int new_SR)
{
	ringmod_.set_sr(sample_rate_);
}

void RingModulator::on_trigger_fired(const rack::Trigger* t)
{
	if (t == trigger_reset_)
	{
		ringmod_.reset(0.0f);
	}
}

void RingModulator::process_left(float in, float* out)
{
	*out = ringmod_.process_left(in);
}

void RingModulator::process_right(float in, float* out)
{
	*out = ringmod_.process_right(in);
}