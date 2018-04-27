#pragma once

#include "ThirdPersonCamera.h"
#include "Camera/CameraModifier.h"
#include "HoatCameraModifierApplyPitchCurves.generated.h"

/** Applies curves in order to modify camera properties based on its current pitch. */
UCLASS(BlueprintType, Blueprintable)
class HOAT_API UHoatCameraModifierApplyPitchCurves : public UCameraModifier
{
    GENERATED_BODY()

public:
    UHoatCameraModifierApplyPitchCurves(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;

private:
    // Curve to convert changes in pitch to changes in camera distance.
    UPROPERTY(Category = Custom, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCurveFloat* PitchToDistanceCurve;

    // Curve to convert changes in pitch to changes in field of view.
    UPROPERTY(Category = Custom, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCurveFloat* PitchToFOVCurve;
};
