#include "HoatCameraModifierInCombat.h"

#include "Camera/CameraTypes.h"

#include "Core/HoatPlayerCharacter.h"

UHoatCameraModifierInCombat::UHoatCameraModifierInCombat(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
}

bool UHoatCameraModifierInCombat::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
    Super::ModifyCamera(DeltaTime, InOutPOV);

    // Get player character.
    AHoatPlayerCharacter* playerCharacter = Cast<AHoatPlayerCharacter>(GetViewTarget());

    if (!IsValid(playerCharacter))
    {
        return false;
    }

    // Apply modifier.
    AppyCameraTransition(playerCharacter->IsInCombat() ? Modifiers : FHoatCameraInfo(), TransitionTime, InOutPOV,
                         DeltaTime);
    return false;
}
