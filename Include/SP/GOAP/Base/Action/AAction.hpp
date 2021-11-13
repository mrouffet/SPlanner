// Copyright(c) 2021 Maxime "mrouffet" ROUFFET. All Rights Reserved.

#pragma once

#ifndef SPLANNER_GOAP_AACTION_GUARD
#define SPLANNER_GOAP_AACTION_GUARD

#include <SP/Abstract/AStep.hpp>

namespace SP
{
	class AAction : public AStep
	{
	public:
		virtual ~AAction() = default;
	};
}

#endif // GUARD
