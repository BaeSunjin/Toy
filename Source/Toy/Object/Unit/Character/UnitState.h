// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Data/DataStore.h"

class ASquad;

/**
 * 
 */
class TOY_API UnitState
{
public:
	explicit UnitState();
	~UnitState();

  Toy::BlackBoard::DataStore<bool> retreat_;

  //TODO: 필요한 정보인지 확인필요
  Toy::BlackBoard::DataStore<ASquad*> attack_target_;
  Toy::BlackBoard::DataStore<bool> run_away_;
  Toy::BlackBoard::DataStore<int> hp_;
  Toy::BlackBoard::DataStore<FVector> move_to_;
  Toy::BlackBoard::DataStore<FVector> look_at_;

};
