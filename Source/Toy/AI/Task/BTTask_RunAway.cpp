// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_RunAway.h"
#include "Object/Unit/Character/UnitBase.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"

UBTTask_RunAway::UBTTask_RunAway(const FObjectInitializer& _objectInitializer)
  : Super(_objectInitializer) {

  NodeName = "RunAway";

  bNotifyTick = true;
  bNotifyTaskFinished = true;
  
}

EBTNodeResult::Type UBTTask_RunAway::ExecuteTask(
                                 UBehaviorTreeComponent& _owner_component,
                                 uint8* _node_memoery) {

  look_at_ = _owner_component.GetBlackboardComponent()->
                              GetValueAsVector("LookAt");

  auto owner = _owner_component.GetBlackboardComponent()->
                                GetValueAsObject("SelfActor");

  auto default_unit = Cast<AUnitBase>(owner);
  auto controller = Cast<AAIController>(_owner_component.GetAIOwner());

  FVector move_pos = default_unit->GetActorLocation() + -look_at_ * 1000.0f;
  controller->MoveTo(move_pos);
 
  return EBTNodeResult::InProgress;

}


void UBTTask_RunAway::TickTask(UBehaviorTreeComponent& _owner_component,
                                uint8* _node_memory,
                                float _delta_second) {
  

  FBTRunAwayTaskMemory* memory = (FBTRunAwayTaskMemory*)_node_memory;
  memory->current_time_ += _delta_second;
  if (memory->current_time_ > destroy_delay_time_) {
    FinishLatentTask(_owner_component, EBTNodeResult::Succeeded);
  }

}

void UBTTask_RunAway::OnTaskFinished(UBehaviorTreeComponent& _onwer_component,
                                      uint8* _node_memory,
                                      EBTNodeResult::Type _task_result) {

  auto owner = Cast<AUnitBase>(_onwer_component.GetBlackboardComponent()->
                                  GetValueAsObject("SelfActor"));

  _onwer_component.StopTree();

  if (owner) {
    owner->Destroy();
  }
  
}


uint16 UBTTask_RunAway::GetInstanceMemorySize() const
{
  return sizeof(FBTRunAwayTaskMemory);
}