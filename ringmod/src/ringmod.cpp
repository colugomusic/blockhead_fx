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
	param_freq_ = add_smooth_param(1000.0f, "Frequency");

	param_freq_->set_format_hint(Rack_ParamFormatHint_Hertz);
	param_freq_->set_min(0.08f);
	param_freq_->set_max(16700.0f);

	param_amount_ = add_smooth_param(100.0f, "Amount");

	param_amount_->set_transform([](const ml::DSPVector& v) { return v / 100.0f; });

	param_amount_->set_size_hint(0.75f);
	param_amount_->set_format_hint(Rack_ParamFormatHint_Percentage);
	param_amount_->set_min(0.0f);
	param_amount_->set_max(100.0f);

	trigger_reset_ = add_trigger("Reset");

	param_freq_->begin_notify();
	param_amount_->begin_notify();
}

void RingModulator::on_trigger_fired(const rack::Trigger* t)
{
	if (t == trigger_reset_)
	{
		ringmod_.reset(0.0f);
	}
}

ml::DSPVectorArray<2> RingModulator::operator()(const ml::DSPVectorArray<2>& in)
{
	return ringmod_(in, sample_rate_, ml::repeat<2>((*param_freq_)()), ml::repeat<2>((*param_amount_)()));
}