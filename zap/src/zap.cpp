#include <rack++/module/param.h>
#include <rack++/module/channel.h>
#include "zap.h"

using namespace rack;

Zap::Zap()
	: BasicStereoEffect("Zap")
{
	param_spread_ = add_param("Spread");
	param_freq_ = add_param("Frequency");
	param_res_ = add_param("Resonance");
	param_mix_ = add_param("Mix");

	param_mix_->set_size_hint(0.75);


	param_spread_->begin_notify();
	param_freq_->begin_notify();
	param_res_->begin_notify();
	param_mix_->begin_notify();
}	

void Zap::on_param_value_changed(const Param* p)
{
	// TODO
}

void Zap::on_sample_rate_changed()
{
	// TODO
}

void Zap::process_left(float in, float* out)
{
	// TODO
	*out = in;
}

void Zap::process_right(float in, float* out)
{
	// TODO
	*out = in;
}