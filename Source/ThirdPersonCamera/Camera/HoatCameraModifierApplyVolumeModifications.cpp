#include "HoatCameraModifierApplyVolumeModifications.h"

#include "Camera/CameraTypes.h"

#include "Camera/CameraActorInterface.h"
#include "Camera/HoatCameraModificationVolume.h"

UHoatCameraModifierApplyVolumeModifications::UHoatCameraModifierApplyVolumeModifications(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
}

bool UHoatCameraModifierApplyVolumeModifications::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
    Super::ModifyCamera(DeltaTime, InOutPOV);

    // Get player character.
	ICameraActorInterface* cameraActor = Cast<ICameraActorInterface>(GetViewTarget());

    if (!cameraActor)
    {
        return false;
    }

    // Apply volume.
    const FHoatCameraInfo playerCameraModificationVolumeInfo = cameraActor->GetCurrentCameraModificationVolume()
        ? cameraActor->GetCurrentCameraModificationVolume()->Modifiers
        : FHoatCameraInfo();

    const float transitionTime = cameraActor->GetCurrentCameraModificationVolume()
        ? cameraActor->GetCurrentCameraModificationVolume()->TransitionTime
        : 0.0f;

    AppyCameraTransition(playerCameraModificationVolumeInfo, transitionTime, InOutPOV, DeltaTime);
    return false;
}
