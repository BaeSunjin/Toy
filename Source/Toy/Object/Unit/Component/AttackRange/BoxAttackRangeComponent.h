// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Object/Unit/Component/AttackRange/AttackRangeInterface.h"
#include "Object/Unit/Component/AttackRange/OverlapObjectContainer.h"
#include "BoxAttackRangeComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOY_API UBoxAttackRangeComponent : public UBoxComponent,
                                         public IAttackRangeInterface
{
	GENERATED_BODY()
	
 public:

   UBoxAttackRangeComponent(const FObjectInitializer & _initializer);

   virtual void Attach(USceneComponent* _parent) override;
   virtual AUnitBase* GetAttackTarget() override;

   virtual void SetTeamFlag(const TeamFlag& _team_flag) override;
   virtual void SetAttackRange(const float& _attack_range) override;
   virtual void Work() override;

   UFUNCTION()
   void BeginOverlapCallBack(UPrimitiveComponent* _overlap_component,
       AActor* _other_actor,
       UPrimitiveComponent* _other_component,
       int32 _other_body_idx,
       bool _from_sweep,
       const FHitResult &_sweep_result);

   UFUNCTION()
   void EndOverlapCallBack(UPrimitiveComponent* _overlap_component,
       AActor* _other_actor,
       UPrimitiveComponent* _other_component,
       int32 _other_body_idx);   

 private:

   
   OverlapObjectContainer<AUnitBase> overlap_object_container_;
   
   USceneComponent* parent_;
   TeamFlag team_flag_;

   float attack_range_;
   float attack_range_squared_;

};
