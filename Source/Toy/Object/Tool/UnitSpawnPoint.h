// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitSpawnPoint.generated.h"

//TODO 위치 변경?
UENUM(Meta = (TeamFlag))
enum class TeamFlag {

  kRed,
  kBlue

};

UCLASS()
class TOY_API AUnitSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitSpawnPoint();

  // return Preferred
  bool IsPermission(const TeamFlag& _flag);
  void SetUse(bool _use);

  //TODO 코드상에서 숨길수 있나?
protected:

  UPROPERTY(EditAnywhere)
  TeamFlag team_flag_;

  UPROPERTY(EditAnywhere)
  bool is_used;
};
