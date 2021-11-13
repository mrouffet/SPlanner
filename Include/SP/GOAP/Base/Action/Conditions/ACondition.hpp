// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_ACONDITION_GUARD
#define SPLANNER_GOAP_ACONDITION_GUARD

namespace SP
{
	class ACondition
	{
	protected:
		virtual bool Validate_Internal(void* _userData) const = 0;

	public:
		/// Should reverse condition.
		bool bReverse = false;

		virtual ~ACondition() = default;

		bool Validate(void* _userData) const;
	};
}

#endif // GUARD
