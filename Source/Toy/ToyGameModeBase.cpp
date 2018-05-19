// Fill out your copyright notice in the Description page of Project Settings.

#include "ToyGameModeBase.h"
#include "Common/PS_Utils.h"
#include "Object/Squad/Squad.h"
#include "Object/Tool/SquadMaker.h"
#include "Object/Tool/UnitSpawnPoint.h"
#include "Object/Unit/Character/DefaultUnit.h"
#include "Object/Unit/Character/UnitTypesInfo.h"
#include "Object/Unit/Character/TestCharacter.h"
#include "Player/Controller/ToyPlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Public/EngineUtils.h"





AToyGameModeBase::AToyGameModeBase(const FObjectInitializer& _objectInitializer)
{

  DefaultPawnClass = ATestCharacter::StaticClass();
  PlayerControllerClass = AToyPlayerController::StaticClass();

  PrimaryActorTick.bCanEverTick = true;
  first = true;

}

void AToyGameModeBase::StartPlay()
{

  Super::StartPlay();

  

}


void AToyGameModeBase::Tick(float _delt) {
  
  if (first == false) return;

  for (int i = 0; i < 10; ++i) {

    int unit_num = 10;
    EPlayerRace unit_type = EPlayerRace::kHuman;
    SquadUnitsInfo test_units_info(10, unit_type);
    TeamFlag team_flag = TeamFlag::kRed;


    // TODO : Spawn 위치를 결정해주는 부분을 다른 부분으로 이동해야한다.
    FVector spawn_pos;
    bool find_spawn_pos = false;
    for (TActorIterator<AUnitSpawnPoint> actor_itr(GetWorld()); actor_itr; ++actor_itr)
    {
      if (!actor_itr->IsPermission(team_flag)) { continue; }

      spawn_pos = actor_itr->GetActorLocation();
      actor_itr->SetUse(true);
      find_spawn_pos = true;
      break;
    }


    SJ_ASSERT(find_spawn_pos);

    ASquad* squad = nullptr;
    SJ_ASSERT(SquadMaker::MakeSquad(test_units_info, spawn_pos,
      FVector2D(1.0f, 0.0f), squad));

    //생성확인.
    SJ_ASSERT(squad);
  }

  first = false;

}