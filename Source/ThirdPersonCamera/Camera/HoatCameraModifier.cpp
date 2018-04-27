#include "HoatCameraModifier.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include "Camera/HoatCameraSpringArmComponent.h"

UHoatCameraModifier::UHoatCameraModifier(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer),
    CurrentModifiers(FHoatCameraInfo()),
    TargetModifiers(FHoatCameraInfo())
{
}

bool UHoatCameraModifier::ProcessViewRotation(class AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
    Super::ProcessViewRotation(ViewTarget, DeltaTime, OutViewRotation, OutDeltaRot);

    if (!IsValid(ViewTarget))
    {
        return false;
    }

    APawn* pawn = Cast<APawn>(ViewTarget);

    if (!IsValid(pawn))
    {
        return false;
    }

    APlayerController* playerController = Cast<APlayerController>(pawn->Controller);

    if (!IsValid(playerController))
    {
        return false;
    }

    if (!playerController->RotationInput.IsNearlyZero(THRESH_QUAT_NORMALIZED))
    {
        // Reset cooldown.
        CooldownRemaining = CooldownAfterPlayerInput;
        return false;
    }

    if (CooldownRemaining > 0)
    {
        // Tick cooldown.
        CooldownRemaining -= DeltaTime;
        return false;
    }

    return false;
}

FHoatCameraInfo UHoatCameraModifier::GetCurrentModifiers() const
{
    return CurrentModifiers;
}

FHoatCameraInfo UHoatCameraModifier::GetTargetModifiers() const
{
    return TargetModifiers;
}

void UHoatCameraModifier::AppyCameraTransition(const FHoatCameraInfo CameraInfo, const float TransitionTime, struct FMinimalViewInfo& InOutPOV, const float DeltaTime)
{
    if (!TargetModifiers.Equals(CameraInfo))
    {
        CurrentModifiers = TargetModifiers;
        TargetModifiers = CameraInfo;

        if (RemainingTransitionTime > 0.0f)
        {
            // New transition started before old was finished. Play transition backwards without flipping.
            RemainingTransitionTime = TotalTransitionTime - RemainingTransitionTime;
        }
    }

    if (CurrentModifiers.Equals(TargetModifiers))
    {
        // No changes.
        ApplyCameraInfo(CurrentModifiers, 1.0f, InOutPOV);
        return;
    }

    // Three cases:
    // 1. CurrentModifiers is zero, TargetModifiers is not zero -> Apply new modifiers.
    // 2. CurrentModifiers is not zero, TargetModifiers is zero -> Stop applying modifiers.
    // 3. CurrentModifiers is not zero, TargetModifiers is not zero -> Need transition from current modifiers to new modifiers.
    // TODO(np): Handle Case 3: Need transition from current modifiers to new modifiers.

    // Apply modifiers.
    float applicationFactor = 0.0f;
    FHoatCameraInfo modifiersToApply;

    if (!TargetModifiers.IsZero())
    {
        // Case 1: Apply new modifiers.
        modifiersToApply = TargetModifiers;

        if (RemainingTransitionTime <= 0.0f)
        {
            // Start transition timer.
            TotalTransitionTime = TransitionTime;
            RemainingTransitionTime = TotalTransitionTime;
        }
        else
        {
            // Tick transition timer.
            RemainingTransitionTime -= DeltaTime;

            if (RemainingTransitionTime <= 0.0f)
            {
                // Transition finished.
                CurrentModifiers = TargetModifiers;
                applicationFactor = 1.0f;
            }
            else
            {
                applicationFactor = 1 - (RemainingTransitionTime / TotalTransitionTime);
            }
        }
    }
    else
    {
        // Case 2: Stop applying modifiers.
        modifiersToApply = CurrentModifiers;

        if (RemainingTransitionTime <= 0.0f)
        {
            // Restart transition timer.
            RemainingTransitionTime = TotalTransitionTime;
            applicationFactor = 1.0f;
        }
        else
        {
            // Tick transition timer.
            RemainingTransitionTime -= DeltaTime;

            if (RemainingTransitionTime <= 0.0f)
            {
                // Transition finished.
                CurrentModifiers = TargetModifiers;
                applicationFactor = 0.0f;
            }
            else
            {
                // This could as well be read as 1 - (1 - (RemainingTransitionTime / TotalTransitionTime)).
                // We want to gradually decrease the influence of the previous modifiers.
                applicationFactor = RemainingTransitionTime / TotalTransitionTime;
            }
        }
    }

    // Apply modifiers.
    ApplyCameraInfo(modifiersToApply, applicationFactor, InOutPOV);
}

bool UHoatCameraModifier::PlayerHasRecentlyChangedCamera() const
{
    return CooldownRemaining > 0;
}

void UHoatCameraModifier::ApplyCameraInfo(const FHoatCameraInfo& CameraInfo, const float Factor, struct FMinimalViewInfo& InOutPOV) const
{
	AActor* viewTarget = GetViewTarget();

    if (!IsValid(viewTarget))
    {
        return;
    }

    TArray<USceneComponent*> children;
	viewTarget->GetRootComponent()->GetChildrenComponents(true, children);

    // Apply FOV.
    float appliedFOV = CameraInfo.FOV * Factor;
    InOutPOV.FOV = InOutPOV.FOV + appliedFOV;

    // Apply spring arm length.
    for (int index = 0; index < children.Num(); ++index)
    {
        UHoatCameraSpringArmComponent* springArmComponent = Cast<UHoatCameraSpringArmComponent>(children[index]);

        if (IsValid(springArmComponent))
        {
            springArmComponent->TargetArmLengthModifier += CameraInfo.SpringArmLength * Factor;
            break;
        }
    }

    // Apply depth of field.
    if (!FMath::IsNearlyZero(CameraInfo.DepthOfFieldFocalDistance))
    {
        InOutPOV.PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
        InOutPOV.PostProcessSettings.DepthOfFieldFocalDistance += CameraInfo.DepthOfFieldFocalDistance * Factor;
    }
    
    if (!FMath::IsNearlyZero(CameraInfo.DepthOfFieldFocalRegion))
    {
        InOutPOV.PostProcessSettings.bOverride_DepthOfFieldFocalRegion = true;
        InOutPOV.PostProcessSettings.DepthOfFieldFocalRegion += CameraInfo.DepthOfFieldFocalRegion * Factor;
    }

    if (!FMath::IsNearlyZero(CameraInfo.DepthOfFieldNearTransitionRegion))
    {
        InOutPOV.PostProcessSettings.bOverride_DepthOfFieldNearTransitionRegion = true;
        InOutPOV.PostProcessSettings.DepthOfFieldNearTransitionRegion += CameraInfo.DepthOfFieldNearTransitionRegion * Factor;
    }

    if (!FMath::IsNearlyZero(CameraInfo.DepthOfFieldFarTransitionRegion))
    {
        InOutPOV.PostProcessSettings.bOverride_DepthOfFieldFarTransitionRegion = true;
        InOutPOV.PostProcessSettings.DepthOfFieldFarTransitionRegion += CameraInfo.DepthOfFieldFarTransitionRegion * Factor;
    }

    if (!FMath::IsNearlyZero(CameraInfo.DepthOfFieldNearBlurSize))
    {
        InOutPOV.PostProcessSettings.bOverride_DepthOfFieldNearBlurSize = true;
        InOutPOV.PostProcessSettings.DepthOfFieldNearBlurSize += CameraInfo.DepthOfFieldNearBlurSize * Factor;
    }

    if (!FMath::IsNearlyZero(CameraInfo.DepthOfFieldFarBlurSize))
    {
        InOutPOV.PostProcessSettings.bOverride_DepthOfFieldFarBlurSize = true;
        InOutPOV.PostProcessSettings.DepthOfFieldFarBlurSize += CameraInfo.DepthOfFieldFarBlurSize * Factor;
    }
}
