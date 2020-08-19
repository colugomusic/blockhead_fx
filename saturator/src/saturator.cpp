#include <rack++/module/param.h>
#include <rack++/module/channel.h>
#include "saturator.h"

using namespace rack;

Saturator::Saturator()
	: BasicStereoEffect("Saturator")
{
	param_drive_ = add_param("Drive");
	param_drive_->set_format_hint(Rack_ParamFormatHint_Percentage);

	param_drive_->begin_notify();
}

void Saturator::on_param_value_changed(const Param* p)
{
	if (p == param_drive_)
	{
		saturator_.set_drive(p->get());
	}
}

void Saturator::process_left(float in, float* out)
{
	*out = saturator_.process_left(in);
}

void Saturator::process_right(float in, float* out)
{
	*out = saturator_.process_right(in);
}