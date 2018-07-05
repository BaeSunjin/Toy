// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Unit/Component/Weapon/WeaponComponentBase.h"
#include "TwoHandedAxComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOY_API UTwoHandedAxComponent : public UWeaponComponentBase
{

  GENERATED_BODY()

 public:
   UTwoHandedAxComponent(const FObjectInitializer & _initializer);
   virtual void Attack(TWeakObjectPtr<AUnitBase> _target) override;

};
