// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Object/Tool/SquadMaker.h"
#include "Object/Unit/Character/UnitTypesInfo.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "CoreMinimal.h"
#include "Senario.generated.h"

USTRUCT(BlueprintType)
struct FSenarioData : public FTableRowBase
{
  GENERATED_USTRUCT_BODY()

public:

  FSenarioData() : stage_(0), stage_name_("nun"), unit_num_(0)
  {}

  // 스테이지 번호
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
  int32 stage_;

  // 스테이지 이름
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
  FString stage_name_;

  // 스테이지 나오는 유닛 이름
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
  int32 unit_num_;

  // stage first unit info
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage",
            Meta = (Bitmask, BitmaskEnum = "EPlayerRace"))
  EPlayerRace unit_race1_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage",
            Meta = (Bitmask, BitmaskEnum = "EPlayerRace"))
  EPlayerRace unit_race2_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage",
            Meta = (Bitmask, BitmaskEnum = "EPlayerRace"))
  EPlayerRace unit_race3_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage",
            Meta = (Bitmask, BitmaskEnum = "EPlayerRace"))
  EPlayerRace unit_race4_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage",
            Meta = (Bitmask, BitmaskEnum = "EPlayerRace"))
  EPlayerRace unit_race5_;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage",
            Meta = (Bitmask, BitmaskEnum = "EPlayerRace"))
  EPlayerRace unit_race6_;

};


/**
 * Enemy들을 생성해준다.
 */
class TOY_API Senario
{
 
private:

	Senario() = delete;

  Senario(const Senario&) = delete;
  Senario& operator = (const Senario&) = delete;

public:

  static void GetSenarioData(const int& stage_,
                             TArray<FSquadUnitsInfo>& _out_info);

};
