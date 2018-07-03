// Fill out your copyright notice in the Description page of Project Settings.

#include "BoxAttackRangeComponent.h"
#include "Common/PS_Utils.h"



// Sets default values for this component's properties
UBoxAttackRangeComponent::UBoxAttackRangeComponent()
{

  PrimaryComponentTick.bCanEverTick = false;
  
  OnComponentBeginOverlap.AddDynamic(this,
    &UBoxAttackRangeComponent::BeginOverlapCallBack);
  OnComponentEndOverlap.AddDynamic(this,
    &UBoxAttackRangeComponent::EndOverlapCallBack);

  bGenerateOverlapEvents = true;
  SetVisibility(true);

  SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  team_flag_ = TeamFlag::kNone;

}

void UBoxAttackRangeComponent::Attach(USceneComponent* _parent) {

  SJ_ASSERT(_parent);
  parent_ = _parent;
  AttachToComponent(_parent,FAttachmentTransformRules::KeepRelativeTransform);
  SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

}

ADefaultUnit* UBoxAttackRangeComponent::GetAttackTarget() {

  SJ_ASSERT((team_flag_ != TeamFlag::kNone));

  //가장 가까운 적을 가지고 온다.
  FVector owner_locatoin = parent_->GetComponentLocation();
  FCloseTarget<ADefaultUnit> target;
  
  bool find = overlap_object_container_.GetCloseTarget(owner_locatoin, target);
  if (!find) { return nullptr; }

  if (target.distance < attack_range_squared_) {
    if (target.object_ptr.IsValid()) {
      return target.object_ptr.Get();
    }  
  }
  
  return nullptr;
  
}


void UBoxAttackRangeComponent::SetTeamFlag(const TeamFlag& _team_flag) {
  team_flag_ = _team_flag;
}

void UBoxAttackRangeComponent::Work() {

  SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
    ECollisionResponse::ECR_Overlap);

}

void UBoxAttackRangeComponent::BeginOverlapCallBack(
  UPrimitiveComponent* _overlap_component,
  AActor* _other_actor,
  UPrimitiveComponent* _other_component,
  int32 _other_body_idx,
  bool _from_sweep,
  const FHitResult &_sweep_result) {

  if (team_flag_ == TeamFlag::kNone) {

    UE_LOG(LogTemp, Error,
      TEXT("BoxAttackRange Not inited but other actor overlap"));

    return;
  }

  auto other_actor = Cast<ADefaultUnit>(_other_actor);
  if (other_actor == nullptr) { return; }

  if (other_actor->GetTeamFlag() == TeamFlag::kNone) {
    UE_LOG(LogTemp, Error,
      TEXT("BoxAttackRange overlaped actor Not inited"));

    return;
  }

  //같은 팀 확인
  if (other_actor->GetTeamFlag() == team_flag_) { return; }

  overlap_object_container_.Add(other_actor);

}


void UBoxAttackRangeComponent::EndOverlapCallBack(
  UPrimitiveComponent* _overlap_component,
  AActor* _other_actor,
  UPrimitiveComponent* _other_component,
  int32 _other_body_idx) {

  auto other_actor = Cast<ADefaultUnit>(_other_actor);
  if (other_actor == nullptr) { return; }

  //같은 팀 확인
  if (other_actor->GetTeamFlag() == team_flag_) { return; }

  overlap_object_container_.Remove(other_actor);

 
}

void UBoxAttackRangeComponent::SetAttackRange(const float& _attack_range) {

  attack_range_ = _attack_range;
  attack_range_squared_ = attack_range_ * attack_range_;

  SetBoxExtent(FVector(attack_range_* 2.0f, attack_range_* 2.0f, 100.0f));

}
