#pragma once

#include "ThirdPersonCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "HoatCameraSpringArmComponent.generated.h"

/**
 * Extends the default Unreal Engine spring arm camera behavior to match the specific needs of HOAT.
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hideCategories = (Mobility))
class UHoatCameraSpringArmComponent : public USpringArmComponent
{
    GENERATED_BODY()

public:
    UHoatCameraSpringArmComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Natural length of the spring arm when there are no collisions. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float BaseTargetArmLength;

    /** Value to add to the target arm length, e.g. caused by camera modifiers. */
    float TargetArmLengthModifier;

protected:
    virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag,
                                          float DeltaTime) override;
};
