# third-person-camera
Sample code of the talk "Six Ingredients for a Dynamic Third-Person Camera" at Unreal Fest Europe 2018.

## Setup

1. Create a HoatPlayerCameraManager blueprint and reference it in your PlayerController blueprint.
1. Ensure you're using the HoatCameraSpringArmComponent at your Character.
1. Implement ICameraActorInterface for your Character.

# Camera Modification Volumes

1. Create a HoatCameraModifierApplyVolumeModifications blueprint and add it to your camera manager.
1. Add HoatCameraModificationVolumes to your levels as desired.

##  License

third-person-camera is dual-licensed:

* everything in Content/Geometry is governed by the Unreal® Engine End User License Agreement
* everything in Content/Mannequin is governed by the Unreal® Engine End User License Agreement
* everything in Content/ThirdPerson is governed by the Unreal® Engine End User License Agreement

Everything else is released under the MIT License.
