// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DefaultUnitController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;
class AUnitBase;

/**
 *
 */
UCLASS()
class TOY_API ADefaultUnitController : public AAIController
{
  GENERATED_BODY()

public:

  

  ADefaultUnitController(const FObjectInitializer& ObjectInitializer);
  virtual void Possess(APawn* _pawn) override;
  virtual void Tick(float _delta_time) override;

  UBlackboardComponent* GetBlackBoardComponent();

  UPROPERTY(VisibleAnywhere, Category = ControllUnit)
  AUnitBase* possess_unit_;

private:
  UPROPERTY(VisibleAnywhere, Category = BlackBoard)
  UBlackboardComponent* blackboard_component_;

  UPROPERTY(VisibleAnywhere, Category = BlackBoard)
  UBehaviorTreeComponent* behavior_tree_component_;

  float black_board_update_timer_;

};
