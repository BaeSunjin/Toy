// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RunAway.generated.h"


struct FBTRunAwayTaskMemory {

  float current_time_;
};

/**
 * 
 */
UCLASS()
class TOY_API UBTTask_RunAway : public UBTTask_BlackboardBase
{
  GENERATED_BODY()


  UBTTask_RunAway(const FObjectInitializer& _objectInitializer);
	
  UPROPERTY(Category = Node, EditAnywhere,
    meta = (ClampMin = "0.0", UIMin = "0.0"))
  float destroy_delay_time_;

  FVector look_at_;

  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _owner_component,
                                  uint8* _node_memoery);

  virtual void TickTask(UBehaviorTreeComponent& , uint8* , float ) override;

  virtual void OnTaskFinished(UBehaviorTreeComponent& ,
                              uint8* , EBTNodeResult::Type ) override;

  virtual uint16 GetInstanceMemorySize() const override;

};