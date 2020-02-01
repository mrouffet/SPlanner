// Copyright 2020 Maxime ROUFFET. All Rights Reserved.

#pragma once


/**
*	Helper macro to call Validate() parent implementation.
*	Should always be called in children Validate() override.
*/
#define SP_DECORATOR_SUPER_VALIDATE(...)\
	if (!Super::Validate_Internal_Implementation(__VA_ARGS__))\
		return false;


/**
*	Helper macro to call Available_Validate() parent implementation.
*	Should always be called in children Available_Validate() override.
*/
#define SP_DECORATOR_SUPER_AVAILABLE(...)\
	if (!Super::Available_Validate_Internal_Implementation(__VA_ARGS__))\
		return false;


/**
*	Helper macro to call PreCondition_Validate() parent implementation.
*	Should always be called in children PreCondition_Validate() override.
*/
#define SP_DECORATOR_SUPER_PRECONDITION(...)\
	if (!Super::PreCondition_Validate_Internal_Implementation(__VA_ARGS__))\
		return false;


/**
*	Helper macro to call Begin_Validate() parent implementation.
*	Should always be called in children Begin_Validate() override.
*/
#define SP_DECORATOR_SUPER_BEGIN(...)\
	if (!Super::Begin_Validate_Internal_Implementation(__VA_ARGS__))\
		return false;