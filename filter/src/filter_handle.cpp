#include "filter_handle.h"
#include "1-pole.h"
#include "2-pole.h"

using namespace rack;

Unit* FilterHandle::make_filter(FilterType type)
{
	switch (type)
	{
		case FilterType::Filter_1Pole:
			return new Filter_1Pole();
		case FilterType::Filter_2Pole:
			return new Filter_2Pole();
		default:
			return nullptr;
	}
}

FilterHandle::FilterHandle(FilterType type)
	: type_(type)
	, unit_(make_filter(type))
{
}

FilterHandle::~FilterHandle()
{
	delete unit_;
}

Unit* FilterHandle::unit()
{
	return unit_;
}

const Unit* FilterHandle::unit() const
{
	return unit_;
}