#include "HoatCameraModifierApplyVolumeModifications.h"

#include "Camera/CameraTypes.h"

#include "Core/HoatPlayerCharacter.h"
#include "Core/Camera/HoatCameraModificationVolume.h"

UHoatCameraModifierApplyVolumeModifications::UHoatCameraModifierApplyVolumeModifications(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
}

bool UHoatCameraModifierApplyVolumeModifications::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
    Super::ModifyCamera(DeltaTime, InOutPOV);

    // Get player character.
    AHoatPlayerCharacter* playerCharacter = Cast<AHoatPlayerCharacter>(GetViewTarget());

    if (!IsValid(playerCharacter))
    {
        return false;
    }

    // Apply volume.
    const FHoatCameraInfo playerCameraModificationVolumeInfo = playerCharacter->GetCurrentCameraModificationVolume()
        ? playerCharacter->GetCurrentCameraModificationVolume()->Modifiers
        : FHoatCameraInfo();

    const float transitionTime = playerCharacter->GetCurrentCameraModificationVolume()
        ? playerCharacter->GetCurrentCameraModificationVolume()->TransitionTime
        : 0.0f;

    AppyCameraTransition(playerCameraModificationVolumeInfo, transitionTime, InOutPOV, DeltaTime);
    return false;
}
