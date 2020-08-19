#include <algorithm>
#include <rack++/module/channel.h>
#include <rack++/module/param.h>
#include <rack++/module/trigger.h>
#include "ringmod.h"

using namespace rack;

RingModulator::RingModulator()
	: BasicStereoEffect("Ring Modulator")
{
	param_freq_ = add_param("Frequency");
	param_freq_->set_format_hint(Rack_ParamFormatHint_Hertz);
	param_freq_->set(600.0f);
	param_freq_->set_min(0.08f);
	param_freq_->set_max(16700.0f);

	param_amount_ = add_param("Amount");
	param_amount_->set_size_hint(0.75f);

	trigger_reset_ = add_trigger("Reset");

	param_freq_->begin_notify();
	param_amount_->begin_notify();
}

void RingModulator::on_param_value_changed(const Param* p)
{
	if (p == param_freq_)
	{
		ringmod_.set_freq(p->get());
	}

	if (p == param_amount_)
	{
		ringmod_.set_amount(p->get());
	}
}

void RingModulator::on_sample_rate_changed()
{
	ringmod_.set_sr(float(sample_rate_));
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