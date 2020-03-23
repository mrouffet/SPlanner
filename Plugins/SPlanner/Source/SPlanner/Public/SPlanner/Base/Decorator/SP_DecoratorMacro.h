// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once


/**
*	Helper macro to call Validate() parent implementation.
*	Should always be called in children Validate() override.
*/
#define SP_DECORATOR_SUPER_VALIDATE(...)\
	if (!Super::Validate_Internal_Implementation(__VA_ARGS__))\
		return false;