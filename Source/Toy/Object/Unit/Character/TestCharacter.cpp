// Fill out your copyright notice in the Description page of Project Settings.

#include "TestCharacter.h"
#include "DefaultUnit.h"
#include "Engine.h"
#include "Common/PS_Utils.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Object/Squad/Squad.h"
#include "Test/TestHelper.h"



// Sets default values
ATestCharacter::ATestCharacter(const FObjectInitializer& _objectInitializer)
{


  // enable Tick function
  PrimaryActorTick.bCanEverTick = true;

  // not needed Pitch Yaw Roll
  bUseControllerRotationPitch = false;
  bUseControllerRotationYaw = false;
  bUseControllerRotationRoll = false;

  // gravity
  GetCharacterMovement()->GravityScale = 0.0f;

  // collision
  GetCapsuleComponent()->bGenerateOverlapEvents = false;
  GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
  GetCapsuleComponent()->SetSimulatePhysics(false);

  // set defaults


  camera_height_ = 1000.f;
  camera_height_min_ = 10.f;                // 100 for debugging
  camera_height_max_ = 5000.f;


  camera_yaw_angle_ = 0.f;                    // yaw
  camera_pitch_angle_ = -70.f;                // pitch

  camera_pitch_angle_min_ = -80.f;
  camera_pitch_angle_max_ = 45.f;

  camera_zoom_speed_ = 200.f;                // wheel
  camera_rotation_speed_ = 1.f;                // wheel + ctrl
  camera_movement_speed_ = 3000.f;            // in all _dirs

  can_camera_move_ = true;
  is_rotation_ = false;

  control_state_ = EControlState::kNon;

  // intialize the camera
  //TODO 80글자 해결
  camera_component_ = _objectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Player Camera"));
  camera_component_->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
  camera_component_->bUsePawnControlRotation = false;
  SetActorRelativeRotation(FRotator(camera_pitch_angle_, camera_yaw_angle_, 0).Quaternion());

}

void ATestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
  if (!PlayerInputComponent) { return; }

  Super::SetupPlayerInputComponent(PlayerInputComponent);

  // action mappings

  // mouse zoom
  PlayerInputComponent->BindAction("ZoomOutByWheel", IE_Pressed, this, &ATestCharacter::ZoomOutByWheel);
  PlayerInputComponent->BindAction("ZoomInByWheel", IE_Pressed, this, &ATestCharacter::ZoomInByWheel);

  // yaw and pitch (mouse right + scroll)
  PlayerInputComponent->BindAction("OnMouseRight", IE_Pressed, this, &ATestCharacter::PressMouseRight);
  PlayerInputComponent->BindAction("OnMouseRight", IE_Released, this, &ATestCharacter::ReleasMouseRight);

  PlayerInputComponent->BindAction("OnMouseLeft", IE_Pressed, this, &ATestCharacter::PressMouseLeft);
  PlayerInputComponent->BindAction("OnMouseLeft", IE_Released, this, &ATestCharacter::ReleasMouseLeft);

  //Pause
  PlayerInputComponent->BindAction("OnPause", IE_Pressed, this, &ATestCharacter::OnPause);
  PlayerInputComponent->BindAction("OffPause", IE_Pressed, this, &ATestCharacter::OffPause);


  // keyboard move (WASD, Home/end)
  PlayerInputComponent->BindAxis("MoveForward", this, &ATestCharacter::MoveCameraForwardInput);
  PlayerInputComponent->BindAxis("MoveRight", this, &ATestCharacter::MoveCameraRightInput);
  PlayerInputComponent->BindAxis("MoveUp", this, &ATestCharacter::MoveCameraUpInput);
  PlayerInputComponent->BindAxis("ZoomIn", this, &ATestCharacter::ZoomCameraInInput);

  // double speed (WASD +Shift)
  PlayerInputComponent->BindAxis("FastMove", this, &ATestCharacter::FastMoveInput);

}


//////////////////////////////////////////////////////////////////


void ATestCharacter::ZoomInByWheel()
{
  if (!can_camera_move_) { return; }

  /*CameraRadius -= CameraZoomSpeed * fast_move_value;
  CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);*/


}


void ATestCharacter::ZoomOutByWheel()
{
  if (!can_camera_move_) { return; }

  /*CameraRadius += CameraZoomSpeed * fast_move_value;
  CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);*/


}



void ATestCharacter::FastMoveInput(float _dir)
{
  if (!can_camera_move_) { return; }

  // left or right does not matter, to set double speed in any _dir
  camera_movement_speed_ = FMath::Abs(_dir) * 2.0f;

  // used as multiplier so must be 1 if not pressed
  if (camera_movement_speed_ == 0.0f)
  {
    camera_movement_speed_ = 1.0f;
  }
}

void ATestCharacter::PressMouseRight() {

  //TODO : 캐릭터의 움직임 처리?
  if (control_state_ == EControlState::kPossessedUnit) {
    MoveSquad();
  }
  else {
    PressRotate();
  }

}

void ATestCharacter::ReleasMouseRight() {

  //TODO : 캐릭터의 움직임 처리?
  if (control_state_ == EControlState::kPossessedUnit) {

  }
  else {
    ReleaseRotate();
  }

}


void ATestCharacter::PressRotate()
{
  if (!can_camera_move_) { return; }

  is_rotation_ = true;

  auto controller = GetWorld()->GetFirstPlayerController();
  bool sucess = controller->GetMousePosition(mouse_pos_.X, mouse_pos_.Y);
  //TODO : execption

}

void ATestCharacter::ReleaseRotate() {

  if (!can_camera_move_) { return; }

  is_rotation_ = false;

}

void ATestCharacter::MoveSquad() {

  FVector mouse_pos;
  FVector camera_dir;

  auto controller = GWorld->GetFirstPlayerController();
  if (controller->DeprojectMousePositionToWorld(mouse_pos, camera_dir))
  {

    FCollisionQueryParams trace_param(FName(TEXT("MouseLay")), false, this);
    trace_param.bFindInitialOverlaps = false;

    FHitResult hit;
    FVector start = FVector(mouse_pos);
    FVector end = FVector(mouse_pos + camera_dir * 5000.0f);

    // ECC_ channels should be set properly !!!
    bool is_hit = GetWorld()->LineTraceSingleByChannel(hit, start, end,
      ECollisionChannel::ECC_WorldStatic, trace_param);

    // notting hit
    if (!is_hit) { return; }

    FVector new_move_pos = hit.Location; // selected object
    if (controlling_squad_.IsValid()) {
      controlling_squad_.Get()->MoveSquad(new_move_pos);
    }

  }
}

bool ATestCharacter::GetSelectingUnit(ADefaultUnit* _out) {

  // mouse data
  FVector mouse_pos;
  FVector camera_dir;

  SJ_ASSERT(GWorld);

  auto controller = GWorld->GetFirstPlayerController();
  SJ_ASSERT(controller);

  //Get Mouse Pos Camera dir
  SJ_ASSERT(controller->DeprojectMousePositionToWorld(mouse_pos, camera_dir));


  FCollisionQueryParams trace_param(FName(TEXT("from Mouse to Lay")),
    false, this);

  trace_param.bFindInitialOverlaps = false;

  FHitResult hit;
  FVector start = FVector(mouse_pos);
  FVector end = FVector(mouse_pos + camera_dir * 5000.0f);

  bool is_hit = GetWorld()->LineTraceSingleByChannel(hit, start, end,
    ECollisionChannel::ECC_MouseRay, trace_param);

  //not hit
  if (!is_hit) { return false; }

  auto unit = Cast<ADefaultUnit>(hit.Actor);

  //not default unit
  if (unit == nullptr) { return false; }
  _out = unit;

  return true;
}

void ATestCharacter::SetNewSquad(const TWeakObjectPtr<ASquad>& _squad) {

  if (controlling_squad_ != nullptr) {
    SJ_ASSERT(controlling_squad_.IsValid());
    controlling_squad_.Get()->SetHighLight(false);
  }

  SJ_ASSERT(_squad.IsValid());

  controlling_squad_ = _squad;
  SJ_ASSERT(controlling_squad_.IsValid());

  controlling_squad_.Get()->SetHighLight(true);

}





void ATestCharacter::PressMouseLeft()
{


  switch (control_state_)
  {
  case EControlState::kUsedMagic:



    break;
  case EControlState::kHighLightUnit:
  case EControlState::kPossessedUnit:
  case EControlState::kNon:
  {
    //Get selected unit to mouse pointer
    ADefaultUnit* unit = nullptr;
    if (!GetSelectingUnit(unit)) { return; }  

    if (controlling_squad_ != nullptr) {
      SJ_ASSERT(controlling_squad_.IsValid());
    }

    SJ_ASSERT(unit->GetSquad().IsValid());

    auto squad_ptr = controlling_squad_.Get();
    auto unit_squad = unit->GetSquad().Get();

    //if reselected squad work nothing
    if (CheckReselect(unit_squad)) { return; }

    SetNewSquad(unit->GetSquad());
    control_state_ = EControlState::kPossessedUnit;


  }
  break;
  default:
    UE_LOG(LogTemp, Error, TEXT("OnMouse Left Out of Switch case"));
    break;
  }

}

void ATestCharacter::ReleasMouseLeft() {

  //Selecteds
  FVector mouse_pos;
  FVector camera_dir;

  bool sucess = GWorld->GetFirstPlayerController()->DeprojectMousePositionToWorld(mouse_pos, camera_dir);
  if (sucess) {

    FCollisionQueryParams trace_param(FName(TEXT("MouseLay")), false, this);        // TraceTag (info for debugging), bTraceComplex, AddIgnoredActor
    trace_param.bFindInitialOverlaps = false;                                        // needed

    FHitResult hit;

    FVector start = FVector(mouse_pos);
    FVector end = FVector(mouse_pos + camera_dir * 1000.0f);

    // ECC_ channels should be set properly !!!
    bool bhit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Pawn, trace_param);
    if (bhit)
    {
      auto a = hit; // selected object
    }

  }
  else {
    //TODO : fix
    return;
  }

}

bool ATestCharacter::CheckReselect(ASquad* _squad) const
{

  if (controlling_squad_.IsValid()) {
    auto squad_ptr = controlling_squad_.Get();

    //if same unit
    if (squad_ptr == _squad) {
      return true;
    }
  }
  return false;

}

void ATestCharacter::OnPause() {
  UE_LOG(LogTemp, Error, TEXT("OnPause"));
  GetWorld()->GetFirstPlayerController()->SetPause(true);

}
void ATestCharacter::OffPause() {
  UE_LOG(LogTemp, Error, TEXT("OffPause"));
  GetWorld()->GetFirstPlayerController()->SetPause(false);
}

void ATestCharacter::MoveCameraForwardInput(float _dir)
{
  if (!can_camera_move_) { return; }
  move_forward_value_ = _dir;
}


void ATestCharacter::MoveCameraRightInput(float _dir)
{
  if (!can_camera_move_) { return; }
  move_right_value_ = _dir;
}


void ATestCharacter::MoveCameraUpInput(float _dir)
{
  if (!can_camera_move_) { return; }
  move_up_value_ = _dir;
}


void ATestCharacter::ZoomCameraInInput(float _dir)
{
  if (!can_camera_move_)    return;
  zoom_in_value_ = _dir;
}


//------------------------------------------------------------


FVector ATestCharacter::MoveCameraForward(float _dir)
{
  float movement_value = _dir * camera_movement_speed_;
  FVector delta_movement = movement_value * GetActorForwardVector();
  //FVector NewLocation = GetActorLocation() + delta_movement;
  //SetActorLocation(NewLocation);

  return delta_movement;
}


FVector ATestCharacter::MoveCameraRight(float _dir)
{
  float movement_value = _dir * camera_movement_speed_;
  FVector delta_movement = movement_value * GetActorRightVector();
  //FVector NewLocation = GetActorLocation() + delta_movement;
  //SetActorLocation(NewLocation);
  return delta_movement;
}


FRotator ATestCharacter::GetIsolatedCameraYaw()
{

  // FRotator containing Yaw only
  return FRotator(
    0.0f, camera_component_->GetComponentToWorld().Rotator().Yaw, 0.0f);
}

//---------------

float ATestCharacter::MoveCameraUp(float _dir)
{
  float movement_value = _dir * camera_movement_speed_;
  return movement_value;
}

//---------------

void ATestCharacter::ZoomCameraIn(float _dir)
{
  float movement_value = _dir * camera_movement_speed_;

}


// (pre mouse pos - mouse pos) -> vector.rotatoin set character
void ATestCharacter::RotateCamera() {


  FVector2D mouse_pos;

  //머지 3D공간상의 좌표를 가지고 오는거 같다.
  auto controller = GetWorld()->GetFirstPlayerController();
  SJ_ASSERT(controller->GetMousePosition(mouse_pos.X, mouse_pos.Y));

  // noting move nothing work
  if (FVector2D::Distance(mouse_pos_, mouse_pos) < 0) { return; }

  FVector2D distance = mouse_pos - mouse_pos_;
  camera_yaw_angle_ += distance.X * camera_rotation_speed_ / 5.0f;

  //Y의 값의 차이를 구한다.
  camera_pitch_angle_ -= distance.Y * camera_rotation_speed_ / 5.0f;
  camera_pitch_angle_ = FMath::Clamp(camera_pitch_angle_, camera_pitch_angle_min_, camera_pitch_angle_max_);

  SetActorRelativeRotation(FRotator(camera_pitch_angle_, camera_yaw_angle_, 0).Quaternion());

  mouse_pos_ = mouse_pos;

}


//////////////////////////////////////////////////////////////////

void ATestCharacter::Tick(float _delta)
{
  Super::Tick(_delta);

  //-------------------
  // tweak camera actor position
  FVector actor_location = GetActorLocation();
  FVector actual_movement = FVector::ZeroVector;

  //움직일 방향 계산.
  actual_movement +=
    MoveCameraForward(move_forward_value_ * camera_movement_speed_ * _delta);

  actual_movement +=
    MoveCameraRight(move_right_value_ * camera_movement_speed_ * _delta);

  actor_location += actual_movement;

  // adjust actor height to surface
  float terrain_surface_z = GetLandTerrainSurfaceAtCoord();
  if ((actor_location.Z - terrain_surface_z) < 20) {

    actor_location.Z = terrain_surface_z + 20.0f;

  }

  // move actor
  SetActorLocation(actor_location);

  if (is_rotation_ == true)
  {
    RotateCamera();
  }

}



//////////////////////////////////////////////////////////////////


float ATestCharacter::GetLandTerrainSurfaceAtCoord() const
{
  FCollisionQueryParams trace_param(FName(TEXT("LandTerrain")), false, this);        // TraceTag (info for debugging), bTraceComplex, AddIgnoredActor
  trace_param.bFindInitialOverlaps = false;                                        // needed

  FHitResult hit;

  FVector actor_pos = GetActorLocation();
  FVector start = FVector(actor_pos.X, actor_pos.Y, GetActorLocation().Z + 300.0f);
  FVector end = FVector(actor_pos.X, actor_pos.Y, -500.f);

  // ECC_ channels should be set properly !!!
  bool bhit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_WorldStatic, trace_param);

  if (bhit)
  {
    return hit.ImpactPoint.Z;    // for shape trace it differs from Location
  }

  return 0.f;        // water level
}
