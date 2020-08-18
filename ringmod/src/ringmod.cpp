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
		const auto hz = param_freq_->get();

		// TODO
	}
}

void RingModulator::on_sample_rate_changed()
{
	// TODO
}

void RingModulator::process_left(float in, float* out)
{
	// TODO
	*out = in;
}

void RingModulator::process_right(float in, float* out)
{
	// TODO
	*out = in;
}