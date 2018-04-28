#pragma once

#include "ThirdPersonCamera.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "TargetingActorInterface.generated.h"

class AActor;

/** Interface for actors that may select other actors as target. */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UTargetingActorInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ITargetingActorInterface
{
	GENERATED_IINTERFACE_BODY()

	virtual bool IsSelectingTarget() const = 0;
	virtual AActor* GetCurrentTarget() const = 0;
	virtual FVector2D GetCurrentTargetSelectionInput() const = 0;
};
