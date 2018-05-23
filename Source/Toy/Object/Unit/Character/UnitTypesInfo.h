#pragma once
#include "UnitTypesInfo.generated.h"

//플레이어 종족 
//TODO 현재 유닛으로 사용중인데 확장이 필요함.
UENUM(BlueprintType)
enum class EPlayerRace : uint8
{

  kHuman = 1 UMETA(DisplayName = "kHuman"),
  kNon = 2   UMETA(DisplayName = "kNon"),

};

