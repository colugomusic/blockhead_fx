#define RACK_EXPORT_MODULE
#include <rack/rack.h>
#include <rack++/module/channel.h>
#include <rack++/module/param.h>
#include <rack++/module/trigger.h>
#include <rack++/module/unit.h>
#include "filter_handle.h"
#include "1-pole.h"
#include "2-pole.h"

const char* rack_get_api_version()
{
	return RACK_API_VERSION;
}

int rack_get_num_units()
{
	return 2;
}

const char* rack_get_module_name()
{
	return "Filters";
}

const char* rack_get_module_version()
{
	return PROJECT_VERSION;
}

const char* rack_get_module_vendor()
{
	return "Blockhead";
}

void* rack_unit_make(int id)
{
	switch (id)
	{
		case 0: return new FilterHandle(FilterType::Filter_1Pole);
		case 1: return new FilterHandle(FilterType::Filter_2Pole);
		default: return nullptr;
	}
}

void rack_unit_free(void* handle)
{
	delete handle;
}

const char* rack_unit_get_name(void* handle)
{
	return ((FilterHandle*)(handle))->unit()->get_name().c_str();
}

void rack_unit_process(void* handle, int num_frames)
{
	((FilterHandle*)(handle))->unit()->process(num_frames);
}

int rack_unit_get_num_params(void* handle)
{
	return ((const FilterHandle*)(handle))->unit()->get_num_params();
}

int rack_unit_get_num_input_channels(void* handle)
{
	return ((const FilterHandle*)(handle))->unit()->get_num_input_channels();
}

int rack_unit_get_num_output_channels(void* handle)
{
	return ((const FilterHandle*)(handle))->unit()->get_num_output_channels();
}

void* rack_unit_get_input_channel(void* handle, int id)
{
	return ((const FilterHandle*)(handle))->unit()->get_input_channel(id);
}

void* rack_unit_get_output_channel(void* handle, int id)
{
	return ((const FilterHandle*)(handle))->unit()->get_output_channel(id);
}

void* rack_unit_get_param(void* handle, int id)
{
	return ((const FilterHandle*)(handle))->unit()->get_param(id);
}

int rack_unit_get_sample_rate(void* handle)
{
	return ((const FilterHandle*)(handle))->unit()->get_sample_rate();
}

void rack_unit_set_sample_rate(void* handle, int sample_rate)
{
	((FilterHandle*)(handle))->unit()->set_sample_rate(sample_rate);
}

char rack_unit_copy(void* dest, void* source)
{
	auto dest_handle = ((FilterHandle*)(dest));
	auto source_handle = ((FilterHandle*)(source));

	switch (dest_handle->type())
	{
		case FilterType::Filter_1Pole:
		{
			if (source_handle->type() != FilterType::Filter_1Pole) return 0;

			((Filter_1Pole*)(dest_handle->unit()))->copy(*((Filter_1Pole*)(source_handle->unit())));
			
			return 1;
		}

		case FilterType::Filter_2Pole:
		{
			if (source_handle->type() != FilterType::Filter_2Pole) return 0;

			((Filter_2Pole*)(dest_handle->unit()))->copy(*((Filter_2Pole*)(source_handle->unit())));

			return 1;
		}
	}
	
	return 0;
}

void rack_unit_reset(void* handle)
{
	((FilterHandle*)(handle))->unit()->reset();
}

const char* rack_param_get_name(void* handle)
{
	return ((rack::Param*)(handle))->get_name().c_str();
}

void rack_param_set_value(void* handle, float value)
{
	((rack::Param*)(handle))->set(value);
}

float rack_param_get_value(void* handle)
{
	return ((rack::Param*)(handle))->get();
}

float rack_param_get_default_value(void* handle)
{
	return ((rack::Param*)(handle))->get_default_value();
}

Rack_ParamFormatHint rack_param_get_format_hint(void* handle)
{
	return ((rack::Param*)(handle))->get_format_hint();
}

float rack_param_get_size_hint(void* handle)
{
	return ((rack::Param*)(handle))->get_size_hint();
}

float rack_param_get_min(void* handle)
{
	return ((rack::Param*)(handle))->get_min();
}

float rack_param_get_max(void* handle)
{
	return ((rack::Param*)(handle))->get_max();
}

const char* rack_param_get_switch_option_name(void* handle, int id)
{
	return ((rack::Param*)(handle))->get_switch_options()[id].c_str();
}

const char* rack_channel_get_name(void* handle)
{
	return ((rack::Channel*)(handle))->name.c_str();
}

void rack_channel_set_buffer_stride(void* handle, int stride)
{
	((rack::Channel*)(handle))->set_buffer_stride(stride);
}

int rack_channel_get_buffer_stride(void* handle)
{
	return ((rack::Channel*)(handle))->get_buffer_stride();
}

char rack_channel_set_input_buffer(void* handle, const float* in)
{
	return ((rack::Channel*)(handle))->set_input_buffer(in);
}

const float* rack_channel_get_input_buffer(void* handle)
{
	return ((rack::Channel*)(handle))->get_input_buffer();
}

char rack_channel_set_output_buffer(void* handle, float* out)
{
	return ((rack::Channel*)(handle))->set_output_buffer(out);
}

float* rack_channel_get_output_buffer(void* handle)
{
	return ((rack::Channel*)(handle))->get_output_buffer();
}

int rack_unit_get_num_triggers(void* handle)
{
	return ((rack::Unit*)(handle))->get_num_triggers();
}

void* rack_unit_get_trigger(void* handle, int id)
{
	return ((rack::Unit*)(handle))->get_trigger(id);
}

const char* rack_trigger_get_name(void* handle)
{
	return ((rack::Trigger*)(handle))->name.c_str();
}

void rack_trigger_fire(void* handle)
{
	((rack::Trigger*)(handle))->fire();
}
