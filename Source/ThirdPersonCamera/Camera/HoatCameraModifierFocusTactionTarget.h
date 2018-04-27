#pragma once

#include "ThirdPersonCamera.h"
#include "Core/Camera/HoatCameraModifier.h"
#include "HoatCameraModifierFocusTactionTarget.generated.h"

/** Makes the camera look in the direction of the current taction target, if there is one. */
UCLASS(BlueprintType, Blueprintable)
class HOAT_API UHoatCameraModifierFocusTactionTarget : public UHoatCameraModifier
{
    GENERATED_BODY()

public:
    UHoatCameraModifierFocusTactionTarget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual bool ProcessViewRotation(class AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation,
                                     FRotator& OutDeltaRot) override;

private:
    /** Rate at which the camera should move while not snapping to a target, in degrees per second. */
    UPROPERTY(Category = Custom, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
    float RotationSpeed;

    /** Rate at which the camera should snap to the target, in degrees per second. */
    UPROPERTY(Category = Custom, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
    float SnapSpeed;

    /** Taction target that was selected at the time of the previous camera update. */
    AActor* LastTarget;

    /** Desired rotation for focusing the taction target. */
    FRotator DesiredRotation;
};
