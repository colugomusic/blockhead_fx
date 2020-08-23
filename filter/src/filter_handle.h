#pragma once

#include "filter_type.h"

namespace rack { class Unit; }

class FilterHandle
{
	FilterType type_;
	rack::Unit* unit_;

	static rack::Unit* make_filter(FilterType type);

public:

	FilterHandle(FilterType type);
	~FilterHandle();

	rack::Unit* unit();
	const rack::Unit* unit() const;
	const FilterType type() const;
};