// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Object/Unit/Character/DefaultUnit.h"
#include "Rule/Rules.h"
#include "AttackRangeInterface.generated.h"

UINTERFACE(BlueprintType)
class TOY_API UAttackRangeInterface : public UInterface
{
  GENERATED_UINTERFACE_BODY()

};

class TOY_API IAttackRangeInterface
{
  GENERATED_IINTERFACE_BODY()

public:
  
  virtual void Attach(USceneComponent* _parent) = 0;
  virtual ADefaultUnit* GetAttackTarget() = 0;
  virtual void SetTeamFlag(const TeamFlag& _team_flag) = 0;
  virtual void SetAttackRange(const float& _attack_range) = 0;
  virtual void Work() = 0;

};