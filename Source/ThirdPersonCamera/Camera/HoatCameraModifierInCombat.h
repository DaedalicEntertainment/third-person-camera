#pragma once

#include "ThirdPersonCamera.h"
#include "Core/Camera/HoatCameraModifier.h"
#include "HoatCameraModifierInCombat.generated.h"

/** Applies modifications while the player is in combat. */
UCLASS(BlueprintType, Blueprintable)
class HOAT_API UHoatCameraModifierInCombat : public UHoatCameraModifier
{
    GENERATED_BODY()

public:
    UHoatCameraModifierInCombat(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;

    /** Time until the modifications are fully applied, in seconds. */
    UPROPERTY(Category = CameraModifiers, BlueprintReadWrite, EditAnywhere)
    float TransitionTime;

    /** Value to add to the current properties of the camera of any player in combat. */
    UPROPERTY(Category = CameraModifiers, BlueprintReadWrite, EditAnywhere)
    FHoatCameraInfo Modifiers;
};
