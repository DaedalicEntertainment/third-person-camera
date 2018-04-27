#include "HoatCameraModifierApplyPitchCurves.h"

#include "Camera/CameraTypes.h"

#include "Camera/HoatPlayerCameraManager.h"

UHoatCameraModifierApplyPitchCurves::UHoatCameraModifierApplyPitchCurves(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
}

bool UHoatCameraModifierApplyPitchCurves::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
    Super::ModifyCamera(DeltaTime, InOutPOV);

    AHoatPlayerCameraManager* cameraManager = Cast<AHoatPlayerCameraManager>(CameraOwner);

    if (!IsValid(cameraManager))
    {
        return false;
    }

    // Get camera location and rotation.
    const FVector cameraLocation = InOutPOV.Location;
    const FRotator cameraRotation = InOutPOV.Rotation;

    // Apply curves.
    const float pitchToDistance = IsValid(PitchToDistanceCurve) ? PitchToDistanceCurve->GetFloatValue(cameraRotation.Pitch) : 0.0f;
    const float pitchToFov = IsValid(PitchToFOVCurve) ? PitchToFOVCurve->GetFloatValue(cameraRotation.Pitch) : 0.0f;

    // Drive camera away based on evaluated curve.
    const FVector desiredLocation =
        cameraLocation - cameraRotation.RotateVector(FVector::ForwardVector) * pitchToDistance;

    FVector newLocation;
    FCollisionQueryParams queryParams(SCENE_QUERY_STAT(SpringArm), false, GetViewTarget());
    FHitResult result;
    GetWorld()->SweepSingleByChannel(result, GetViewTarget()->GetActorLocation(), desiredLocation, FQuat::Identity,
                                     cameraManager->LineOfSightProbeChannel,
                                     FCollisionShape::MakeSphere(cameraManager->LineOfSightProbeSize), queryParams);
    newLocation = result.bBlockingHit ? result.Location : desiredLocation;

    InOutPOV.Location = newLocation;
    InOutPOV.FOV += pitchToFov;
    return false;
}
