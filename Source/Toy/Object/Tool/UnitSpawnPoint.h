// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitSpawnPoint.generated.h"

//TODO ��ġ ����?
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

  //TODO �ڵ�󿡼� ����� �ֳ�?
protected:

  UPROPERTY(EditAnywhere)
  TeamFlag team_flag_;

  UPROPERTY(EditAnywhere)
  bool is_used;
};
