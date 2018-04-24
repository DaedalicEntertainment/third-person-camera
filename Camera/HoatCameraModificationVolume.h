#pragma once

#include "Hoat.h"
#include "Engine/TriggerVolume.h"
#include "Core/Camera/HoatCameraInfo.h"
#include "HoatCameraModificationVolume.generated.h"

/** Volume that modifies the camera of entering players. */
UCLASS()
class AHoatCameraModificationVolume : public ATriggerVolume
{
    GENERATED_BODY()

public:
    AHoatCameraModificationVolume(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Sets the currently applied camera modification volume for the specified actor. */
    virtual void NotifyActorBeginOverlap(AActor* Other) override;

    // Resets the currently applied camera modification volume for the specified actor. */
    virtual void NotifyActorEndOverlap(AActor* Other) override;

    /** Time until the modifications are fully applied, in seconds. */
    UPROPERTY(Category = CameraModifiers, BlueprintReadWrite, EditAnywhere)
    float TransitionTime;

    /** Value to add to the current properties of the camera of any player entering the volume. */
    UPROPERTY(Category = CameraModifiers, BlueprintReadWrite, EditAnywhere)
    FHoatCameraInfo Modifiers;
};
