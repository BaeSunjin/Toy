// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestCharacter.generated.h"

class ADefaultUnit;
class ASquad;
class UCameraComponent;


UENUM()
enum class EControlState : uint8 {


  kUsedMagic = 0,
  kHighLightUnit,
  kPossessedUnit,
  kNon

};

// TODO 이름 변경 필요
// 일단 캐릭터로 하고
// MainCamera

UCLASS()
class TOY_API ATestCharacter : public ACharacter
{
  GENERATED_BODY()

public:
  // Sets default values for this character's properties
  ATestCharacter(const FObjectInitializer& _objectInitializer);

  //-------- camera -------

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* camera_component_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float camera_height_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float camera_height_min_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float camera_height_max_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float camera_yaw_angle_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float camera_pitch_angle_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
  float camera_pitch_angle_min_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float camera_pitch_angle_max_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float camera_zoom_speed_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float camera_rotation_speed_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float camera_movement_speed_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool can_camera_move_;
                             
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
  bool camera_fast_move_;

  // --------- root boody (character) movement

  float move_forward_value_;
  float move_right_value_;
  float move_up_value_;
  float zoom_in_value_;






  // controled squad : (unit controll manager) 
  TWeakObjectPtr<ASquad> controlling_squad_;

private:


  void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);


private:

  

  EControlState control_state_;


  // Input event recode
  bool is_rotation_;

  FVector2D mouse_pos_;


public:

  void ZoomInByWheel();
  void ZoomOutByWheel();

  void PressFastMoveInput();
  void ReleasFastMoveInput();

  /** press right mouse button */
  void PressMouseRight();

  /** releas right mouse button */
  void ReleasMouseRight();

  /** On Mouse Right Event at Unit Released state camera rotate */
  void PressRotate();

  /** Off Mouse Right Event at Unit Released state camera rotate */
  void ReleaseRotate();

  /** On Pocessed squad **/
  void MoveSquad();

  /** presse left mouse button **/
  void PressMouseLeft();

  /** Release left mouse button **/
  void ReleasMouseLeft();

  bool CheckReselect(ASquad* _squad) const;


  void OnPause();
  void OffPause();

  /** Input recieved to move the camera forward
  * @param direcation - (1.0 for forward, -1.0 for backward)
  */
  void MoveCameraForwardInput(float _dir);

  /** Input recieved to move the camera right
  * @param direcation - (1.0 for right, -1.0 for left)
  */
  void MoveCameraRightInput(float _dir);

  /** Input recieved to move the camera right
  * @param direcation - (1.0 for right, -1.0 for left)
  */
  void MoveCameraUpInput(float _dir);

  /** Input recieved to move the camera right
  * @param direcation - (1.0 for right, -1.0 for left)
  */
  void ZoomCameraInInput(float _dir);


private:

  /** Moves the camera forward
  * @param direcation - (+ forward, - backward)
  */
  FVector MoveCameraForward(float _dir);

  /** Moves the camera right
  * @param direcation - (+ right, - left)
  */
  FVector MoveCameraRight(float _dir);

  /** Gets the roatation of the camera with only the yaw value
  * @return - returns a rotator that is (0, yaw, 0) of the Camera
  */
  FRotator GetIsolatedCameraYaw();



  /** Moves the camera up/down
  * @param direcation - (+ up, - down)
  */
  float MoveCameraUp(float _dir);

  //---

  /** Zooms the camera in/out
  * @param direcation - (+ in, - out)
  */
  void ZoomCameraIn(float _dir);

  void RotateCamera();

  bool GetSelectingUnit(ADefaultUnit*& _out);
  void SetNewSquad(const TWeakObjectPtr<ASquad>& _squad);

public:

  /** Tick Function, handles keyboard inputs */
  virtual void Tick(float _delat) override;

  //------------------------------------

  // detect landscape and terrain static-mesh
  // usage: RTS Obstacle and RTS Building placement onto landscape, terrain static-mesh
  float    GetLandTerrainSurfaceAtCoord() const;

  //------------------------------------
};
