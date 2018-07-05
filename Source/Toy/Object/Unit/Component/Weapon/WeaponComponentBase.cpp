// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponentBase.h"
#include "Common/PS_Utils.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UWeaponComponentBase::UWeaponComponentBase(
  const FObjectInitializer & _initializer) : Super(_initializer)
{
  
}

void UWeaponComponentBase::Init() {
  InitMesh();
}

void UWeaponComponentBase::AttachTo(USkeletalMeshComponent* _mesh,
                                  FName _socket_name) {

  SJ_ASSERT(_mesh);
  AttachToComponent(_mesh, FAttachmentTransformRules::KeepRelativeTransform,
    _socket_name);

}
void UWeaponComponentBase::Attack(TWeakObjectPtr<AUnitBase> _target) {

}

float UWeaponComponentBase::GetDemage() {
  return demage_;
}
void UWeaponComponentBase::SetDemage(float _demage) {
  demage_ = _demage;
}

void UWeaponComponentBase::InitMesh() {

  if (static_mesh_file_path_.Len() == 0) {

    UE_LOG(LogTemp, Error,
      TEXT("%s initfaile not setting static mesh file path"), *GetName());
    SJ_ASSERT(false);
  }

  static ConstructorHelpers::FObjectFinder<USkeletalMesh>
    weapon_mesh(*static_mesh_file_path_);

  SJ_ASSERT(weapon_mesh.Object);

  SetSkeletalMesh(weapon_mesh.Object);
  SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
  RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
  SetCollisionEnabled(ECollisionEnabled::NoCollision);

}