#include "HoatCameraInfo.h"

bool FHoatCameraInfo::Equals(const FHoatCameraInfo& Other) const
{
    FHoatCameraInfo delta;
    delta.FOV = FOV - Other.FOV;
    delta.SpringArmLength = SpringArmLength - Other.SpringArmLength;
    delta.DepthOfFieldFocalDistance = DepthOfFieldFocalDistance - Other.DepthOfFieldFocalDistance;
    delta.DepthOfFieldFocalRegion = DepthOfFieldFocalRegion - Other.DepthOfFieldFocalRegion;
    delta.DepthOfFieldNearTransitionRegion = DepthOfFieldNearTransitionRegion - Other.DepthOfFieldNearTransitionRegion;
    delta.DepthOfFieldFarTransitionRegion = DepthOfFieldFarTransitionRegion - Other.DepthOfFieldFarTransitionRegion;
    delta.DepthOfFieldNearBlurSize = DepthOfFieldNearBlurSize - Other.DepthOfFieldNearBlurSize;
    delta.DepthOfFieldFarBlurSize = DepthOfFieldFarBlurSize - Other.DepthOfFieldFarBlurSize;
    return delta.IsZero();
}

bool FHoatCameraInfo::IsZero() const
{
    return FMath::Abs(FOV) < SMALL_NUMBER && FMath::Abs(SpringArmLength) < SMALL_NUMBER &&
        FMath::Abs(DepthOfFieldFocalDistance) < SMALL_NUMBER && FMath::Abs(DepthOfFieldFocalRegion) < SMALL_NUMBER &&
        FMath::Abs(DepthOfFieldNearTransitionRegion) < SMALL_NUMBER &&
        FMath::Abs(DepthOfFieldFarTransitionRegion) < SMALL_NUMBER &&
        FMath::Abs(DepthOfFieldNearBlurSize) < SMALL_NUMBER && FMath::Abs(DepthOfFieldFarBlurSize) < SMALL_NUMBER;
}
