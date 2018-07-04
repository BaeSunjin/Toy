// Fill out your copyright notice in the Description page of Project Settings.

#include "TwoHandedAxComponent.h"
#include "Object/Unit/Character/DefaultUnit.h"
#include "Object/Squad/Squad.h"

UTwoHandedAxComponent::UTwoHandedAxComponent(
  const FObjectInitializer & _initializer) :
  Super(_initializer) {

  static_mesh_file_path_ = "SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Axes/Blade_AnthraciteAxe/SK_Blade_AnthraciteAxe.SK_Blade_AnthraciteAxe'";
  Init();

}

void UTwoHandedAxComponent::Attack(TWeakObjectPtr<ADefaultUnit> _target) {

  if (_target.IsValid()) {
    auto squad = _target.Get()->GetSquad();
    if (squad.IsValid()) {
      squad.Get()->RemoveUnit(_target.Get());
    }

    _target.Get()->Destroy();
  }

}