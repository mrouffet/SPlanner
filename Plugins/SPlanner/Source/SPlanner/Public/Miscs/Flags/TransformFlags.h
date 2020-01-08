#pragma once

/** Flags used during plan generation for Transform. */
UENUM(BlueprintType, Blueprintable, Category = "SPlanner|Misc|Flags")
enum class ESP_TransformFlags : uint8
{
	/** Position has changed. */
	TF_DirtyPosition		UMETA(Display = "DirtyPosition"),

	/** Position has changed. */
	TF_DirtyRotation		UMETA(Display = "DirtyRotation"),
};

ENUM_CLASS_FLAGS(ESP_TransformFlags)