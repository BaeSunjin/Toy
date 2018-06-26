// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadMaker.h"
#include "Common/PS_Utils.h"
#include "Engine.h"
#include "Object/Squad/Squad.h"
#include "Object/Unit/Character/DefaultUnit.h"


class SquadUnitMaker {

public:

  static void CreateSquadUnits(const FSquadUnitsInfo& _squad_units_info,
                               const TeamFlag& _team_flag,
                               ASquad* _squad);

private:

  static void CreateUnits(const FSquadUnitsInfo& _squad_units_info,
                          ASquad* _squad);

  static void InitUnitsInfo(const TeamFlag& _team_flag, ASquad* _squad);

  //race : 종족
  static UClass* GetGenerateClass(const EPlayerRace& _race);



  SquadUnitMaker();

};


void SquadUnitMaker::CreateSquadUnits(const FSquadUnitsInfo& _squad_units_info,
                                      const TeamFlag& _team_flag,
                                      ASquad* _squad)
{

  SJ_ASSERT(_squad);
  CreateUnits(_squad_units_info, _squad);
  InitUnitsInfo(_team_flag , _squad);

}

void SquadUnitMaker::CreateUnits(const FSquadUnitsInfo& _squad_units_info,
                                 ASquad* _squad) {

  SJ_ASSERT(GWorld);
  SJ_ASSERT(_squad);

  // 유닛 생성후 TArray<TWeakObjectPtr<ADefaultUnit>> 에 저장한다.
  auto generate_class = GetGenerateClass(_squad_units_info.unit_type_);
  SJ_ASSERT(generate_class);

  for (int i = 0; i < _squad_units_info.unit_num_; i++) {
    if (generate_class != nullptr) {
      auto new_unit = GWorld->SpawnActor(generate_class);
      auto cunverted_pointer = Cast<ADefaultUnit>(new_unit);
      _squad->units_.Add(cunverted_pointer);
    }
  }

}

void SquadUnitMaker::InitUnitsInfo(const TeamFlag& _team_flag,
                                   ASquad* _squad) {

  SJ_ASSERT(_squad);

  for (auto unit : _squad->units_) {
    unit->SetSquad(_squad);
    unit->SetTeamFlag(_team_flag);
  }


  //유닛의 OverlapEvent Work
  for (auto unit : _squad->units_) {
    unit->WorkAttackComponent();
  }

  

}

UClass* SquadUnitMaker::GetGenerateClass(const EPlayerRace& _race)
{
  UClass* _ret = nullptr;
  switch (_race)
  {
  case EPlayerRace::kHuman:
    _ret = ADefaultUnit::StaticClass();
    break;
  default:

    // out switch range
    SJ_ASSERT(false);
    break;
  }

  return _ret;
}


FSquadUnitsInfo::FSquadUnitsInfo(const int& _unit_num,
                               const EPlayerRace& _unit_type)
  : unit_num_(_unit_num), unit_type_(_unit_type)
{

  SJ_ASSERT(_unit_num);

}

ASquad* SquadMaker::MakeSquad(const FSquadUnitsInfo& _info,
                              const TeamFlag& _team_flag)
{
  
  SJ_ASSERT(GWorld);
  auto squad = GWorld->SpawnActor<ASquad>();

  //TODO 대열에 관하여 작성해야됨
  squad->array_vertical_ = 5;
  squad->array_horizental_ = 2;

  SquadUnitMaker::CreateSquadUnits(_info, _team_flag , squad);

  return squad;
}



