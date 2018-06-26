// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultWeaponComponent.h"
#include "Common/PS_Utils.h"
#include "Object/Unit/Character/DefaultUnit.h"
#include "Object/Squad/Squad.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"



UDefaultWeaponComponent::UDefaultWeaponComponent(
  const FObjectInitializer & _initializer)
  : Super(_initializer)
{

  InitMesh();

}

void UDefaultWeaponComponent::InitMesh() {

  static ConstructorHelpers::FObjectFinder<USkeletalMesh> weapon_mesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Axes/Blade_AnthraciteAxe/SK_Blade_AnthraciteAxe.SK_Blade_AnthraciteAxe'"));
  SJ_ASSERT(weapon_mesh.Object);

  SetSkeletalMesh(weapon_mesh.Object);
  SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
  RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
  SetCollisionEnabled(ECollisionEnabled::NoCollision);
 
}

void UDefaultWeaponComponent::Attach(USkeletalMeshComponent* _mesh,
                                     FName _socket_name) {
  
  SJ_ASSERT(_mesh);
  AttachToComponent(_mesh,
    FAttachmentTransformRules::KeepRelativeTransform, _socket_name);

}

void UDefaultWeaponComponent::AttackTarget(
  TWeakObjectPtr<ADefaultUnit> _target) {

  if (_target.IsValid()) {
    auto squad = _target.Get()->GetSquad();
    if (squad.IsValid()) {
      squad.Get()->RemoveUnit(_target.Get());
    }

    _target.Get()->Destroy();
  }

}