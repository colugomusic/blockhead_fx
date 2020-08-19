#include <rack++/module/param.h>
#include <rack++/module/channel.h>
#include "saturator.h"

using namespace rack;

Saturator::Saturator()
	: BasicStereoEffect("Saturator")
{
	param_drive_ = add_param("Drive");
	param_drive_->set_format_hint(Rack_ParamFormatHint_Percentage);

	param_ceiling_ = add_param("Ceiling");
	param_ceiling_->set_format_hint(Rack_ParamFormatHint_Decibels);
	param_ceiling_->set_min(-60.0f);
	param_ceiling_->set_max(0.0f);
	param_ceiling_->set_size_hint(0.75f);

	param_drive_->begin_notify();
	param_ceiling_->begin_notify();
}

void Saturator::on_param_value_changed(const Param* p)
{
	if (p == param_drive_)
	{
		saturator_.set_drive(p->get());
	}

	if (p == param_ceiling_)
	{
		saturator_.set_ceiling_db(p->get());
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