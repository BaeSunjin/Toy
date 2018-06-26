// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultUnitController.h"
#include "Common/PS_Utils.h"
#include "Object/Unit/Character/DefaultUnit.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Navigation/CrowdFollowingComponent.h"


ADefaultUnitController::ADefaultUnitController(const FObjectInitializer& ObjectInitializer) 
  : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent"))) 
{

  PrimaryActorTick.bCanEverTick = true;

  blackboard_component_ = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
  behavior_tree_component_ = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponenet"));
  
}

void ADefaultUnitController::Possess(APawn* _pawn) {
  Super::Possess(_pawn);

  possess_unit_ = Cast<ADefaultUnit>(_pawn);
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

  if (!possess_unit_->state_.changed_) { return; }

  if (possess_unit_->state_.run_away_.Value == true) {

    blackboard_component_->
      SetValueAsBool("RunAway", possess_unit_->state_.run_away_.Key);

    possess_unit_->state_.run_away_.Value = false;
    possess_unit_->state_.changed_ = false;

    UE_LOG(LogTemp, Error, TEXT("Set Run Away"));

  }

  if (possess_unit_->state_.hp_.Value == true) {

    blackboard_component_->
      SetValueAsInt("Hp", possess_unit_->state_.hp_.Key);

    possess_unit_->state_.hp_.Value = false;
    possess_unit_->state_.changed_ = false;

  }

  if (possess_unit_->state_.move_to_.Value == true) {

    blackboard_component_->
      SetValueAsVector("MoveTo", possess_unit_->state_.move_to_.Key);

    possess_unit_->state_.move_to_.Value = false;
    possess_unit_->state_.changed_ = false;

  }

  if (possess_unit_->state_.look_at_.Value == true) {

    blackboard_component_->
      SetValueAsVector("LookAt", possess_unit_->state_.look_at_.Key);

    possess_unit_->state_.look_at_.Value = false;
    possess_unit_->state_.changed_ = false;

  }

  if (possess_unit_->state_.retreat_.Value == true) {

    blackboard_component_->
      SetValueAsBool("Retreat", possess_unit_->state_.retreat_.Key);
    possess_unit_->state_.retreat_.Value = false;
    possess_unit_->state_.changed_ = false;

  }

  

}
