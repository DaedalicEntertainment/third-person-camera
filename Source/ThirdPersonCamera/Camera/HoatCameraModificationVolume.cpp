#include "HoatCameraModificationVolume.h"

#include "Camera/CameraActorInterface.h"

AHoatCameraModificationVolume::AHoatCameraModificationVolume(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    TransitionTime = 1.0f;
}

void AHoatCameraModificationVolume::NotifyActorBeginOverlap(AActor* Other)
{
    Super::NotifyActorBeginOverlap(Other);

	ICameraActorInterface* cameraActor = Cast<ICameraActorInterface>(Other);

    if (!cameraActor)
    {
        return;
    }

    // Set camera modification volume.
	cameraActor->SetCurrentCameraModificationVolume(this);
}

void AHoatCameraModificationVolume::NotifyActorEndOverlap(AActor* Other)
{
    Super::NotifyActorEndOverlap(Other);

	ICameraActorInterface*  cameraActor = Cast<ICameraActorInterface>(Other);

    if (!cameraActor)
    {
        return;
    }

    // Reset camera modification volume.
    if (cameraActor->GetCurrentCameraModificationVolume() == this)
    {
		cameraActor->SetCurrentCameraModificationVolume(nullptr);
    }
}
