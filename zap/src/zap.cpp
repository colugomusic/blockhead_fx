#include <rack++/module/smooth_param.h>
#include <rack++/module/channel.h>
#include "zap.h"

using namespace rack;

Zap::Zap()
	: BasicStereoEffect("Zap")
{
	param_spread_ = add_smooth_param("Spread");
	param_freq_ = add_smooth_param("Frequency");
	param_res_ = add_smooth_param("Resonance");
	param_mix_ = add_smooth_param("Mix");

	param_spread_->set_size_hint(0.75);
	param_res_->set_size_hint(0.75);
	param_mix_->set_size_hint(0.5);

	param_spread_->begin_notify();
	param_freq_->begin_notify();
	param_res_->begin_notify();
	param_mix_->begin_notify();
}

void Zap::on_sample_rate_changed(int new_SR)
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