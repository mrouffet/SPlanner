// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_ACONDITION_GUARD
#define SPLANNER_GOAP_ACONDITION_GUARD

namespace SP
{
	class ACondition
	{
	public:
		virtual ~ACondition() = default;

		virtual bool Validate() const = 0;
	};
}

#endif // GUARD
