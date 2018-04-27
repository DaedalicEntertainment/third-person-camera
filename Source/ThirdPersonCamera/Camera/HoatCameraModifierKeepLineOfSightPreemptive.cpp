#include "HoatCameraModifierKeepLineOfSightPreemptive.h"

#include "Camera/PlayerCameraManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Camera/HoatPlayerCameraManager.h"

UHoatCameraModifierKeepLineOfSightPreemptive::UHoatCameraModifierKeepLineOfSightPreemptive(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    MaximumAngleInRadians = PI / 2;
    StepSizeInRadians = 0.1f;

    RotationSpeed = 1.0f;

    LineOfSightProbeChannel = ECC_Camera;
}

bool UHoatCameraModifierKeepLineOfSightPreemptive::ProcessViewRotation(class AActor* ViewTarget, float DeltaTime,
                                                                       FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
    Super::ProcessViewRotation(ViewTarget, DeltaTime, OutViewRotation, OutDeltaRot);

    if (PlayerHasRecentlyChangedCamera())
    {
        return false;
    }

    // Get the camera location and rotation.
    const FVector desiredLocation = CameraOwner->GetCameraLocation();
    const FRotator desiredRotation = CameraOwner->GetCameraRotation();

    // Check if target is visible for the camera. Shoot "whiskers" to the left and right.
    const FVector targetLocation = GetViewTarget()->GetActorLocation();

    float desiredAngleInRadians = 0.0f;
    float checkAngleInRadians = 0.0f;

    // Check from the left. Start very close to the actual camera angle:
    // This is the most severe case, in which we might need to make a big swing away.
    checkAngleInRadians = -StepSizeInRadians;

    while (checkAngleInRadians >= -MaximumAngleInRadians)
    {
        FVector testLocation = desiredLocation;
        FRotator testRotation = desiredRotation;

        RotateAroundLocation(targetLocation, testLocation, testRotation, checkAngleInRadians);

        // Check if line of sight blocked.
        if (!IsInLineOfSight(targetLocation, testLocation))
        {
            // We found an obstacle for this angle.
            // The smaller the absolute value of CheckAngleInRadians,
            // the closer the obstacle, and the further we need to swing away.
            desiredAngleInRadians += checkAngleInRadians + MaximumAngleInRadians;
            break;
        }

        // Look further away.
        checkAngleInRadians -= StepSizeInRadians;
    }

    // Check from right.
    checkAngleInRadians = StepSizeInRadians;

    while (checkAngleInRadians <= MaximumAngleInRadians)
    {
        FVector testLocation = desiredLocation;
        FRotator testRotation = desiredRotation;

        RotateAroundLocation(targetLocation, testLocation, testRotation, checkAngleInRadians);

        // Check if line of sight blocked.
        if (!IsInLineOfSight(targetLocation, testLocation))
        {
            desiredAngleInRadians += checkAngleInRadians - MaximumAngleInRadians;
            break;
        }

        // Look further away.
        checkAngleInRadians += StepSizeInRadians;
    }

    // Apply rotation.
    const float appliedAngleInRadians =
        FMath::Clamp(desiredAngleInRadians, -RotationSpeed * DeltaTime, +RotationSpeed * DeltaTime);

    OutDeltaRot.Yaw += FMath::RadiansToDegrees(appliedAngleInRadians);

    return false;
}

bool UHoatCameraModifierKeepLineOfSightPreemptive::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
    Super::ModifyCamera(DeltaTime, InOutPOV);

    return false;
}

bool UHoatCameraModifierKeepLineOfSightPreemptive::IsInLineOfSight(const FVector& From, const FVector& To) const
{
    UWorld* world = GetWorld();
    if (!IsValid(world))
    {
        return false;
    }

    AHoatPlayerCameraManager* cameraManager = Cast<AHoatPlayerCameraManager>(CameraOwner);

    if (!IsValid(cameraManager))
    {
        return false;
    }

    FCollisionQueryParams queryParams(SCENE_QUERY_STAT(SpringArm), false, GetViewTarget());
    FHitResult result;
    world->SweepSingleByChannel(result, From, To, FQuat::Identity, LineOfSightProbeChannel,
                                FCollisionShape::MakeSphere(cameraManager->LineOfSightProbeSize), queryParams);

    return !result.bBlockingHit;
}

void UHoatCameraModifierKeepLineOfSightPreemptive::RotateAroundLocation(const FVector& TargetLocation,
                                                                        FVector& InOutViewLocation,
                                                                        FRotator& InOutViewRotation,
                                                                        const float AngleInRadians)
{
    // Add angle.
    FRotator newRotation = InOutViewRotation;
    newRotation.Yaw += FMath::RadiansToDegrees(AngleInRadians);
    InOutViewRotation = newRotation;

    // Rotate test location around pawn.
    FVector newLocation = InOutViewLocation;

    const float angleSin = FMath::Sin(AngleInRadians);
    const float angleCos = FMath::Cos(AngleInRadians);

    // Translate point to origin.
    newLocation.X -= TargetLocation.X;
    newLocation.Y -= TargetLocation.Y;

    // Rotate point.
    const float rotatedX = newLocation.X * angleCos - newLocation.Y * angleSin;
    const float rotatedY = newLocation.X * angleSin + newLocation.Y * angleCos;

    // Translate point back.
    newLocation.X = rotatedX + TargetLocation.X;
    newLocation.Y = rotatedY + TargetLocation.Y;

    InOutViewLocation = newLocation;
}
