// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultUnitController.h"
#include "Common/PS_Utils.h"
#include "Object/Unit/Character/UnitBase.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Navigation/CrowdFollowingComponent.h"


#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"

const static float BLACKBOARD_UPDATE_TIME = 0.5f;

ADefaultUnitController::ADefaultUnitController(const FObjectInitializer& ObjectInitializer) 
  : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) 
{

  PrimaryActorTick.bCanEverTick = true;

  blackboard_component_ = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
  behavior_tree_component_ = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponenet"));
  black_board_update_timer_ = 0.0f;
}

void ADefaultUnitController::Possess(APawn* _pawn) {
  Super::Possess(_pawn);

  possess_unit_ = Cast<AUnitBase>(_pawn);
  SJ_ASSERT(possess_unit_);

  blackboard_component_->
    InitializeBlackboard(*(possess_unit_->GetUnitBlackBoardData()));
  blackboard_component_->SetValueAsObject("SelfActor", possess_unit_);
  blackboard_component_->SetValueAsVector("LookAt",
                                      possess_unit_->GetActorForwardVector());

  behavior_tree_component_->StartTree(*(possess_unit_->GetUnitBehavior()));

}

void ADefaultUnitController::Tick(float _delta_time) {
  Super::Tick(_delta_time);

  black_board_update_timer_ += _delta_time;
  if (black_board_update_timer_ < BLACKBOARD_UPDATE_TIME) { return; }
  else { black_board_update_timer_ = 0.0f; }

  auto unit_state = possess_unit_->GetState();
  
 
  Toy::BlackBoard::PushToBlackBoard<int, UBlackboardKeyType_Int>(unit_state.hp_, *this);
  Toy::BlackBoard::PushToBlackBoard<FVector,UBlackboardKeyType_Vector>(unit_state.look_at_, *this);
  Toy::BlackBoard::PushToBlackBoard<FVector, UBlackboardKeyType_Vector>(unit_state.move_to_, *this);
  Toy::BlackBoard::PushToBlackBoard<bool, UBlackboardKeyType_Bool>(unit_state.retreat_, *this);
  Toy::BlackBoard::PushToBlackBoard<bool, UBlackboardKeyType_Bool>(unit_state.run_away_, *this);
  
}

UBlackboardComponent* ADefaultUnitController::GetBlackBoardComponent() {
  return blackboard_component_;
}