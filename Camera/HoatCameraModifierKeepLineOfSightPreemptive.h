#pragma once

#include "Hoat.h"
#include "Core/Camera/HoatCameraModifier.h"
#include "Engine/EngineTypes.h"
#include "HoatCameraModifierKeepLineOfSightPreemptive.generated.h"

/**
 * Makes the camera try to always keep line of sight to its target.
 * Preemptively swings away from obstacles before they occlude the character.
 */
UCLASS(BlueprintType, Blueprintable)
class HOAT_API UHoatCameraModifierKeepLineOfSightPreemptive : public UHoatCameraModifier
{
    GENERATED_BODY()

public:
    UHoatCameraModifierKeepLineOfSightPreemptive(
        const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual bool ProcessViewRotation(class AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation,
                                     FRotator& OutDeltaRot) override;
    virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;

    /** How far away from the current camera angle we should look for potential obstacles. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float MaximumAngleInRadians;

    /** How precisely we should preemptively look for obstacles. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float StepSizeInRadians;

    /** Rate at which the camera should follow the turning character, in radians per second. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float RotationSpeed;

    /** Collision channel of the query probe for verifying line of sight between camera and view target. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    TEnumAsByte<ECollisionChannel> LineOfSightProbeChannel;

private:
    /** Checks whether To can be seen from From, with respect to ProbeSize and ProbeChannel. */
    bool IsInLineOfSight(const FVector& From, const FVector& To) const;

    /** Rotates InOutViewLocation and InOutViewRotation around TargetLocation by AngleInRadians. */
    void RotateAroundLocation(const FVector& TargetLocation, FVector& InOutViewLocation, FRotator& InOutViewRotation,
                              const float AngleInRadians);
};
