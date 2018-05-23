// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rule/Rules.h"
#include "UnitSpawnPoint.generated.h"


UCLASS()
class TOY_API AUnitSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitSpawnPoint();

protected:

  UPROPERTY(VisibleDefaultsOnly)
  USceneComponent* root_component_;

  //TODO �ڵ�󿡼� ����� �ֳ�?
  UPROPERTY(EditAnywhere)
  TeamFlag team_flag_;

  UPROPERTY(EditAnywhere)
  bool is_used;


public:

  // return Preferred
  bool IsPermission(const TeamFlag& _flag);
  void SetUse(bool _use);

};
