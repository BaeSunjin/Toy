// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Object/Unit/Component/AttackRangeInterface.h"
#include <mutex>
#include "BoxAttackRangeComponent.generated.h"

//원기둥으로 변경필요!!!!
class UBoxComponent;
class ADefaultUnit;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOY_API UBoxAttackRangeComponent : public UActorComponent,
                                         public IAttackRangeInterface
{

	GENERATED_BODY()



 public:
  
	// Called every frame
   virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

   virtual void Attach(USceneComponent* _parent) override;
   virtual ADefaultUnit* GetAttackTarget() override;

   virtual void SetTeamFlag(const TeamFlag& _team_flag) override;
   virtual void  SetAttackRange(const float& _attack_range) override;
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

 protected:

   UBoxAttackRangeComponent();
   virtual void BeginPlay() override;

private:

   bool HasAlreadyAdded(ADefaultUnit* _actor);


   UPROPERTY(VisibleDefaultsOnly, Category = AttackRange)
    UBoxComponent* box_collision_;

   UPROPERTY(VisibleAnywhere, Category = AttackRange)
   TArray<TWeakObjectPtr<ADefaultUnit>> targets_;

   USceneComponent* parent_;
   TeamFlag team_flag_;
   float attack_range_;
   float attack_range_squared_;

   std::mutex mutex_;
   static int end_overlap_call_count_;

};
