#include "HoatCameraModifierFocusTactionTarget.h"

#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

#include "Core/Tactions/TactionStateComponent.h"

UHoatCameraModifierFocusTactionTarget::UHoatCameraModifierFocusTactionTarget(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    RotationSpeed = 300.0f;
    SnapSpeed = 50.0f;
}

bool UHoatCameraModifierFocusTactionTarget::ProcessViewRotation(class AActor* ViewTarget, float DeltaTime,
                                                                FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
    Super::ProcessViewRotation(ViewTarget, DeltaTime, OutViewRotation, OutDeltaRot);

    // Check if we're in taction state.
    if (!IsValid(ViewTarget))
    {
        return false;
    }

    UTactionStateComponent* tactionState = ViewTarget->FindComponentByClass<UTactionStateComponent>();

    if (!IsValid(tactionState))
    {
        return false;
    }

    if (!tactionState->IsTactionStateEnabled())
    {
        return false;
    }

    // Check if there's a taction target.
    AActor* currentTarget = tactionState->GetCurrentTarget();

    if (IsValid(currentTarget))
    {
        if (currentTarget != LastTarget)
        {
            // We have selected a new target. Compute desired rotation.
            FVector cameraLocation = CameraOwner->GetCameraLocation();
            FVector targetLocation = currentTarget->GetActorLocation();

            // Get rotation delta from current camera rotation to look-at rotation for the target.
            // As the camera position might change depending on its rotation (e.g. for spring arms),
            // we just compute this once per target and store the result.
            // Otherwise, for very close targets, we risk rotating the camera back and forth as its position changes.
            DesiredRotation = UKismetMathLibrary::FindLookAtRotation(cameraLocation, targetLocation);
            LastTarget = currentTarget;
        }

        FRotator currentRotation = OutViewRotation;

        if (SnapSpeed <= 0)
        {
            // Snap immediately.
            OutViewRotation = DesiredRotation;
        }
        else
        {
            FRotator towardsDesired = DesiredRotation - currentRotation;
            FRotator towardsDesiredNormalized = towardsDesired.GetNormalized();

            if (towardsDesiredNormalized.IsNearlyZero())
            {
                // Prevent overshooting.
                OutViewRotation = DesiredRotation;
            }
            else
            {
                // Apply rotation speed.
                FRotator deltaRot = towardsDesiredNormalized * SnapSpeed * DeltaTime;
                OutDeltaRot += deltaRot;
            }
        }
    }
    else
    {
        LastTarget = nullptr;

        // No target selected. Smoothly apply player input.
        FVector2D targetSelectionInput = tactionState->GetCurrentTargetSelectionInput();

        FRotator deltaRot;
        deltaRot.Yaw = targetSelectionInput.X * RotationSpeed * DeltaTime;
        deltaRot.Pitch = -targetSelectionInput.Y * RotationSpeed * DeltaTime;
        deltaRot.Roll = 0.0f;

        OutDeltaRot += deltaRot;
    }

    // Prevent further camera modifiers from being applied.
    return true;
}
