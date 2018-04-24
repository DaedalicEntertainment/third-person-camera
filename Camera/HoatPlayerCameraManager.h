#pragma once

#include "Hoat.h"
#include "Camera/PlayerCameraManager.h"
#include "HoatPlayerCameraManager.generated.h"

/** Defines the final view properties for the HOAT camera. */
UCLASS(notplaceable, transient, BlueprintType, Blueprintable)
class HOAT_API AHoatPlayerCameraManager : public APlayerCameraManager
{
    GENERATED_BODY()

public:
    AHoatPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Radius of the query probe for verifying line of sight between camera and view target (in cm). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float LineOfSightProbeSize;

    /** Collision channel of the query probe for verifying line of sight between camera and view target. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    TEnumAsByte<ECollisionChannel> LineOfSightProbeChannel;

protected:
    virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
