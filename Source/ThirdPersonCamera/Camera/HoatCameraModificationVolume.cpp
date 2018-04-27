#include "HoatCameraModificationVolume.h"

#include "Core/HoatPlayerCharacter.h"

AHoatCameraModificationVolume::AHoatCameraModificationVolume(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    TransitionTime = 1.0f;
}

void AHoatCameraModificationVolume::NotifyActorBeginOverlap(AActor* Other)
{
    Super::NotifyActorBeginOverlap(Other);

    AHoatPlayerCharacter* playerCharacter = Cast<AHoatPlayerCharacter>(Other);

    if (!IsValid(playerCharacter))
    {
        return;
    }

    // Set camera modification volume.
    playerCharacter->SetCurrentCameraModificationVolume(this);

    UE_LOG(hoat, Log, TEXT("Applied camera modification volume %s for player character %s."), *GetName(),
        *playerCharacter->GetName());
}

void AHoatCameraModificationVolume::NotifyActorEndOverlap(AActor* Other)
{
    Super::NotifyActorEndOverlap(Other);

    AHoatPlayerCharacter* playerCharacter = Cast<AHoatPlayerCharacter>(Other);

    if (!IsValid(playerCharacter))
    {
        return;
    }

    // Reset camera modification volume.
    if (playerCharacter->GetCurrentCameraModificationVolume() == this)
    {
        playerCharacter->SetCurrentCameraModificationVolume(nullptr);

        UE_LOG(hoat, Log, TEXT("Reset camera modification volume for player character %s."),
            *playerCharacter->GetName());
    }
}
