// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rule/Rules.h"
#include "Object/Unit/Character/UnitTypesInfo.h"


//������ ������ �� ���
//������ ���ڿ� Ÿ�� ������ ������
//Unit Maker�� ������ ���ڿ� Ÿ�Կ� �°� ����
struct FSquadUnitsInfo
{
  
  int unit_num_;
  EPlayerRace unit_type_;

  FSquadUnitsInfo() = delete;
  FSquadUnitsInfo(const int& _unit_num, const EPlayerRace& _unit_type);
  
};

class ADefaultUnit;
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

  //���� ����
  SquadMaker() = delete;

  SquadMaker(const SquadMaker&) = delete;
  SquadMaker& operator= (const SquadMaker&) = delete;




};
