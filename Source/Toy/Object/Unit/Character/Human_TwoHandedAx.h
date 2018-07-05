// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Unit/Character/UnitBase.h"
#include "Human_TwoHandedAx.generated.h"

/**
 * 
 */
UCLASS()
class TOY_API AHuman_TwoHandedAx : public AUnitBase
{
	GENERATED_BODY()
	
 public:
   AHuman_TwoHandedAx(const FObjectInitializer & _initializer);
	
   virtual void OnDamage(int _damega);
   virtual void Tick(float _delta_time) override;


};
