#include <rack++/module/param.h>
#include <rack++/module/channel.h>
#include "saturator.h"

using namespace rack;

Saturator::Saturator()
	: BasicStereoEffect("Saturator")
{
	param_drive_ = add_param("Drive");

	param_ceiling_ = add_param("Ceiling");
	param_ceiling_->set(1.0f);

	param_drive_->begin_notify();
	param_ceiling_->begin_notify();
}

void Saturator::on_param_value_changed(const Param* p)
{
	// TODO
}

void Saturator::on_sample_rate_changed()
{
	// TODO
}

void Saturator::process_left(float in, float* out)
{
	// TODO
	*out = in;
}

void Saturator::process_right(float in, float* out)
{
	// TODO
	*out = in;
}