# third-person-camera
Sample code of the talk "Six Ingredients for a Dynamic Third-Person Camera" at Unreal Fest Europe 2018.

## Setup

1. Create a HoatPlayerCameraManager blueprint and reference it in your PlayerController blueprint.
1. Ensure you're using the HoatCameraSpringArmComponent at your Character.
1. Implement ICameraActorInterface and IPlayerCharacterInterface for your Character.

## Usage
### Camera Modification Volumes

1. Create a HoatCameraModifierApplyVolumeModifications blueprint and add it to your camera manager.
1. Add HoatCameraModificationVolumes to your levels as desired.

### Pitch-To-Distance Curves

1. Create a curve asset.
1. Create a HoatCameraModifierApplyPitchCurves blueprint and add it to your camera manager.

### Focusing Walk Directions

1. Create a HoatCameraModifierFocusWalkDirection blueprint and add it to your camera manager.

### Keeping Line of Sight Preemptively

1. Add a new collision trace channel (e.g. CameraRotateAround).
1. Create a HoatCameraModifierKeepLineOfSightPreemptive blueprint, use your new trace channel, and add the modifier to your camera manager.
1. Block the new trace channel for all objects in your level the camera should rotate around. Make sure to ignore the default Camera trace channel to prevent the spring arm from kicking in.

## License

third-person-camera is dual-licensed:

* everything in Content/Geometry is governed by the Unreal® Engine End User License Agreement
* everything in Content/Mannequin is governed by the Unreal® Engine End User License Agreement
* everything in Content/ThirdPerson is governed by the Unreal® Engine End User License Agreement

Everything else is released under the MIT License.
