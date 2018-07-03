// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Unit/Character/DefaultUnit.h"
#include "Human_Archer.generated.h"

/**
 * 
 */
UCLASS()
class TOY_API AHuman_Archer : public ADefaultUnit
{
  GENERATED_BODY()

 public:

   AHuman_Archer();


   // Called every frame
   virtual void Tick(float _delta_time) override;

};
