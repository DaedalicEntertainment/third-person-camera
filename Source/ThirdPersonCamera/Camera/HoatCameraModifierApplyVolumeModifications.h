#pragma once

#include "ThirdPersonCamera.h"
#include "Camera/HoatCameraModifier.h"
#include "HoatCameraModifierApplyVolumeModifications.generated.h"

class AHoatCameraModificationVolume;

/** Applies modifications caused by level volumes the player enters. */
UCLASS(BlueprintType, Blueprintable)
class UHoatCameraModifierApplyVolumeModifications : public UHoatCameraModifier
{
    GENERATED_BODY()

public:
    UHoatCameraModifierApplyVolumeModifications(
        const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;
};
