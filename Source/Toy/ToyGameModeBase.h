// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToyGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class TOY_API AToyGameModeBase : public AGameModeBase
{
  GENERATED_BODY()

  AToyGameModeBase(const FObjectInitializer& _objectInitializer);

  bool first;

  virtual void StartPlay() override;
  virtual void Tick(float _delta) override;

};
