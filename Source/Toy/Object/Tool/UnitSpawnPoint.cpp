// Fill out your copyright notice in the Description page of Project Settings.
#include "UnitSpawnPoint.h"


// Sets default values
AUnitSpawnPoint::AUnitSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

  root_component_ = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
  root_component_->SetRelativeLocation(FVector(0, 0, 0));
  SetRootComponent(root_component_);

  SetActorHiddenInGame(false);

}

bool AUnitSpawnPoint::IsPermission(const TeamFlag& _flag)
{
  if (is_used) { return false; }

  //ÆÀ ±¸ºÐ
  return team_flag_ == _flag;

}

void AUnitSpawnPoint::SetUse(bool _use) {
  is_used = _use;
}