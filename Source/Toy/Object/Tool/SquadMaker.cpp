// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadMaker.h"
#include "Common/PS_Utils.h"
#include "Engine.h"
#include "Object/Squad/Squad.h"
#include "Object/Unit/Character/DefaultUnit.h"


class SquadUnitMaker {

private:

  SquadUnitMaker();


public:

  static void CreateSquadUnit(const FSquadUnitsInfo& _squad_units_info,
                              TArray<TWeakObjectPtr<ADefaultUnit>>& _out);

  //race : 종족
  static bool ToGenerateClass(const EPlayerRace& _race, UClass*& _out);

};


void SquadUnitMaker::CreateSquadUnit(const FSquadUnitsInfo& _squad_units_info,
                                    TArray<TWeakObjectPtr<ADefaultUnit>>& _out)
{

  SJ_ASSERT(GWorld);

  // 유닛 생성후 TArray<TWeakObjectPtr<ADefaultUnit>> 에 저장한다.
  UClass* generate_class = nullptr;
  SJ_ASSERT(ToGenerateClass(_squad_units_info.unit_type_, generate_class));
  SJ_ASSERT(generate_class);

  for (int i = 0; i < _squad_units_info.unit_num_; i++) {
    if (generate_class != nullptr) {
      auto new_unit = GWorld->SpawnActor(generate_class);
      auto cunverted_pointer = Cast<ADefaultUnit>(new_unit);
      _out.Add(cunverted_pointer);
    }
  }

}


bool SquadUnitMaker::ToGenerateClass(const EPlayerRace& _race, UClass*& _out)
{

  switch (_race)
  {
  case EPlayerRace::kHuman:

    _out = ADefaultUnit::StaticClass();
    return true;

  default:

    // out switch range
    SJ_ASSERT(false);
    break;
  }

  return false;
}


FSquadUnitsInfo::FSquadUnitsInfo(const int& _unit_num,
                               const EPlayerRace& _unit_type)
  : unit_num_(_unit_num), unit_type_(_unit_type)
{

  SJ_ASSERT(_unit_num);

}

bool SquadMaker::MakeSquad(const FSquadUnitsInfo& _info,
                           const FVector& _spawn_pos,
                           const FVector2D& _squad_forward,
                           ASquad*& _out)
{
 
  SJ_ASSERT(GWorld);
  _out = GWorld->SpawnActor<ASquad>();

  SJ_ASSERT(_squad_forward.Size() == 1.0f);
  SquadUnitMaker::CreateSquadUnit(_info, _out->units_);

  for (auto unit : _out->units_) {
    unit->SetSquad(_out);
  }

  //TODO 대열에 관하여 작성해야됨
  _out->array_vertical_ = 5;
  _out->array_horizental_ = 2;

  _out->Init(_spawn_pos, _squad_forward);
 
  return true;
}



