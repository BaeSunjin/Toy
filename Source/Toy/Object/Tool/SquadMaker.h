// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Unit/Character/UnitTypesInfo.h"

//유닛을 생성할 때 사용
//유닛의 숫자와 타입 정보를 가지고
//Unit Maker가 유닛의 숫자와 타입에 맞게 생성

struct SquadUnitsInfo
{

  int unit_num_;
  EPlayerRace unit_type_;

  SquadUnitsInfo(const int& _unit_num, const EPlayerRace& _unit_type);

};

class ADefaultUnit;
class ASquad;

/**
*
*/
class TOY_API SquadMaker
{

private:

  //생성 금지
  SquadMaker();


public:
  
  static bool MakeSquad(const SquadUnitsInfo& _info,
                        const FVector& _spawn_pos,
                        const FVector2D& _squad_forward,
                        ASquad*& _out);

};
