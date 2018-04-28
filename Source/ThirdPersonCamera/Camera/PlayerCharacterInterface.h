#pragma once

#include "ThirdPersonCamera.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "PlayerCharacterInterface.generated.h"

/** Interface for actors that may receive player input. */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UPlayerCharacterInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IPlayerCharacterInterface
{
	GENERATED_IINTERFACE_BODY()

	virtual bool GotMovementInput() const = 0;
};
