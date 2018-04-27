#include "HoatPlayerCameraManager.h"

#include "EngineUtils.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"

AHoatPlayerCameraManager::AHoatPlayerCameraManager(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    LineOfSightProbeSize = 12.0f;
    LineOfSightProbeChannel = ECC_Camera;
}

void AHoatPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
    Super::UpdateViewTarget(OutVT, DeltaTime);
}
