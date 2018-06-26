// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveAndWaitEnemy.generated.h"

class ADefaultUnit;
struct FBTMoveAndWaitEnemy {

  TWeakObjectPtr<ADefaultUnit> self_;
  float range_;
  FVector move_pos_;

};

/**
 * 
 */
UCLASS()
class TOY_API UBTTask_MoveAndWaitEnemy : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
    UBTTask_MoveAndWaitEnemy(const FObjectInitializer& _objectInitializer);
  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _owner_component,
    uint8* _node_memoery);
	
  virtual void TickTask(UBehaviorTreeComponent&, uint8*, float) override;
  virtual uint16 GetInstanceMemorySize() const override;

};
