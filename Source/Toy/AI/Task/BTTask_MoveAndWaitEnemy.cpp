// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveAndWaitEnemy.h"
#include "Object/Unit/Character/DefaultUnit.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/Core/Public/Math/NumericLimits.h"



//������ �ϴ� ����� ���Ͽ�
//���� ����� ������ �ٰ��� �����Ѵ�.
UBTTask_MoveAndWaitEnemy::UBTTask_MoveAndWaitEnemy(const FObjectInitializer& _objectInitializer)
  : Super(_objectInitializer) {

  NodeName = "MoveAndWaitEnemy";

  bNotifyTick = true;
  bNotifyTaskFinished = true;

}


EBTNodeResult::Type UBTTask_MoveAndWaitEnemy::ExecuteTask(
  UBehaviorTreeComponent& _owner_component,
  uint8* _node_memoery) {


  auto attack_range = _owner_component.GetBlackboardComponent()->
    GetValueAsFloat("AttackRange");

  auto self_ptr = _owner_component.GetBlackboardComponent()->
    GetValueAsObject("SelfActor");

  auto default_unit_ptr = Cast<ADefaultUnit>(self_ptr);  
    
  auto move_to = _owner_component.GetBlackboardComponent()->
    GetValueAsVector("MoveTo");


  FBTMoveAndWaitEnemy* memory = (FBTMoveAndWaitEnemy*)_node_memoery;
  memory->self_ = default_unit_ptr;
  memory->range_ = attack_range;
  memory->move_pos_ = move_to;
  

  //���ο� �̵� ��ġ ����� ������ �ʾ�����
  if (move_to.X == TNumericLimits< float >::Max()) {
    return EBTNodeResult::Succeeded;
  }

  Cast<AAIController>(default_unit_ptr->GetController())->MoveTo(move_to);
  return EBTNodeResult::InProgress;

}

void UBTTask_MoveAndWaitEnemy::TickTask(UBehaviorTreeComponent& _owner_component,
  uint8* _node_memory,
  float _delta_second) {

  FBTMoveAndWaitEnemy* memory = (FBTMoveAndWaitEnemy*)_node_memory;
  
  
  if (memory->self_.IsValid()) {
    auto self_ref = memory->self_.Get();
    
    //������ ���� �߰ߵǾ��ٸ�
    if (self_ref->GetAttackTarget() != nullptr) {
      Cast<AAIController>(self_ref->GetController())->StopMovement();
      FinishLatentTask(_owner_component, EBTNodeResult::Succeeded);
      
    }
    
    //���ο� �������� ����ٸ�.
    auto new_move_to = _owner_component.GetBlackboardComponent()->
      GetValueAsVector("MoveTo");

    if (memory->move_pos_ != new_move_to) {
      FinishLatentTask(_owner_component, EBTNodeResult::Succeeded);
    }
  }
}

uint16 UBTTask_MoveAndWaitEnemy::GetInstanceMemorySize() const
{
  return sizeof(FBTMoveAndWaitEnemy);
}