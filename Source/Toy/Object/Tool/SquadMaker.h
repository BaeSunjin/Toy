// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rule/Rules.h"
#include "Object/Unit/Character/UnitTypesInfo.h"


//유닛을 생성할 때 사용
//유닛의 숫자와 타입 정보를 가지고
//Unit Maker가 유닛의 숫자와 타입에 맞게 생성
struct FSquadUnitsInfo
{
  
  int unit_num_;
  EPlayerRace unit_type_;

  FSquadUnitsInfo() = delete;
  FSquadUnitsInfo(const int& _unit_num, const EPlayerRace& _unit_type);
  
};


class ASquad;

/**
*
*/
class TOY_API SquadMaker
{

public:

  static ASquad* MakeSquad(const FSquadUnitsInfo& _info,
                           const TeamFlag& _team_flag);

  

private:

  //생성 금지
  SquadMaker() = delete;

  SquadMaker(const SquadMaker&) = delete;
  SquadMaker& operator= (const SquadMaker&) = delete;




};
