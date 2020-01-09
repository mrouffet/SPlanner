#include <Components/SP_POIComponent.h>

USP_POIComponent::USP_POIComponent(const FObjectInitializer& ObjectInitializer)
{
	SphereRadius = 50.0f;
}

USP_POIActionSet* USP_POIComponent::GetActionSet() const
{
	return ActionsSet;
}