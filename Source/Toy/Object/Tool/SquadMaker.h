// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

private:

  //���� ����
  SquadMaker() = delete;


public:
  
  static bool MakeSquad(const FSquadUnitsInfo& _info,
                        const FVector& _spawn_pos,
                        const FVector2D& _squad_forward,
                        ASquad*& _out);

};
