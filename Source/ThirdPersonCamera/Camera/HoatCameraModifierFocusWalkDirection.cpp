#include "HoatCameraModifierFocusWalkDirection.h"

#include "GameFramework/Actor.h"
#include "Camera/PlayerCharacterInterface.h"


UHoatCameraModifierFocusWalkDirection::UHoatCameraModifierFocusWalkDirection(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    RotationSpeed = 10.0f;
}

bool UHoatCameraModifierFocusWalkDirection::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
    //UE_LOG(hoat, Log, TEXT("Location: %s, Rotation: %s, FOV: %f"), *InOutPOV.Location.ToString(), *InOutPOV.Rotation.Euler().ToString(), InOutPOV.FOV);
    return Super::ModifyCamera(DeltaTime, InOutPOV);
}

bool UHoatCameraModifierFocusWalkDirection::ProcessViewRotation(class AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
    Super::ProcessViewRotation(ViewTarget, DeltaTime, OutViewRotation, OutDeltaRot);

    // Tick cooldown.
    if (DirectionChangeCooldownRemaining > 0)
    {
        DirectionChangeCooldownRemaining -= DeltaTime;
    }

	IPlayerCharacterInterface* playerCharacter = Cast<IPlayerCharacterInterface>(ViewTarget);

    if (!playerCharacter)
    {
        return false;
    }

    // Check if we should apply automatic rotation.
    if (RotateOnlyWhileCharacterIsMoving && !playerCharacter->GotMovementInput())
    {
        return false;
    }

    if (PlayerHasRecentlyChangedCamera())
    {
        return false;
    }

    // Get current actor and view rotations.
    const float actorYaw = ViewTarget->GetActorRotation().Yaw;
    const float viewYaw = OutViewRotation.Yaw;

    // Always take the "short route" while rotating.
    float yawDelta = actorYaw - viewYaw;

    while (yawDelta < -180.0f)
    {
        yawDelta += 360.0f;
    }

    while (yawDelta > 180.0f)
    {
        yawDelta -= 360.0f;
    }

    // Check direction of rotation.
    float yawDeltaSign = FMath::Sign(yawDelta);

    if (PreviousYawDeltaSign != yawDeltaSign)
    {
        if (DirectionChangeCooldownRemaining > 0)
        {
            return false;
        }
        else
        {
            PreviousYawDeltaSign = yawDeltaSign;
            DirectionChangeCooldownRemaining = DirectionChangeCooldown;
        }
    }

    // Apply rotation speed.
    float appliedYawDelta = yawDeltaSign * RotationSpeed * DeltaTime;

    // Prevent flipping back and forth for very small deltas.
    if (FMath::Abs(yawDelta) < FMath::Abs(appliedYawDelta))
    {
        PreviousYawDeltaSign = 0.0f;
        DirectionChangeCooldownRemaining = DirectionChangeCooldown;
        appliedYawDelta = yawDelta;
    }

    OutDeltaRot.Yaw += appliedYawDelta;

    return false;
}
