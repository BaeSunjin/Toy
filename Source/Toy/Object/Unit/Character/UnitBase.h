// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/SerialNum.h"
#include "GameFramework/Character.h"
#include "Object/Unit/Character/UnitState.h"
#include "Rule/Rules.h"
#include "UnitBase.generated.h"

class IAttackRangeInterface;
class UWeaponComponentBase;

const static float ATTACK_TARGET_UPDATE_TIME = 1.0f;

UCLASS()
class TOY_API AUnitBase : public ACharacter, public SerialNum<AUnitBase>
{
	GENERATED_BODY()

 public:
   // Sets default values for this character's properties
   AUnitBase(const FObjectInitializer & _initializer);
   // call all init function
   void Init(UWeaponComponentBase* _weapon,
             IAttackRangeInterface* _attack_range);

   void WorkAttackComponent();

   const TWeakObjectPtr<ASquad>& GetSquad();
   float GetHirzontalInterval();
   float GetVerticalInterval();
   AUnitBase* GetAttackTarget();
   TeamFlag GetTeamFlag();
   const UnitState& GetState();

   void SetHighLight(bool _light_on);
   void SetAttackTarget(ASquad* _target);
   void SetAttackRange(float _attack_range);


   bool ExistMovePath();

   void MoveTo(const FVector& _move_pos,
               const FVector& _normal_look_at,
               const bool& _retreat);

   UFUNCTION(BlueprintPure, Category = "AttackTarget")
   bool IsExistAttackTarget();

   UFUNCTION(BlueprintCallable, Category = "AttackTarget")
   void Attack();

   UFUNCTION(BlueprintCallable, Category = "AttackTarget")
   void LookAt();

   virtual void OnDamage(int _damega);
   virtual void Tick(float _delta_time) override;


   void RunAway();

 protected:
   // Called when the game starts or when spawned
   virtual void BeginPlay() override;


 private:

#if WITH_EDITOR
   virtual void HelperUIInit();
#endif

   void InitCollision();
   void InitHLMesh();
   void InitMesh();
   void InitController();
   void InitAnimBlueprint();
   void InitBehaviorTree();
   void InitState();
   void InitInterval(float _vertical, float _horizontal);
   void SetAttackRangeComponent(IAttackRangeInterface* _new_attack_range,
                                float _attack_range);

   void SetWeapon(UWeaponComponentBase* _new_weapon,
                  USkeletalMeshComponent* _mesh,
                  FString _socket_name);

   //SquadMaker에서만 사용을 한다.
   friend class SquadUnitMaker;
   void SetSquad(const TWeakObjectPtr<ASquad>& data);
   void SetTeamFlag(const TeamFlag& _team_flag);


  


   //ADefaultUnitController 에서만 접근 허용
   friend class ADefaultUnitController;
   UPROPERTY(EditAnywhere, Category = Behavior)
   UBehaviorTree* behavior_;
   UBlackboardData* GetUnitBlackBoardData();
   UBehaviorTree* GetUnitBehavior();

   UPROPERTY(EditAnywhere, Category = HighLight)
   UStaticMeshComponent* high_light_component_;


   UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
   USkeletalMeshComponent* unit_mesh_;

   UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
   UWeaponComponentBase* weapon_component_;

   UPROPERTY(VisibleDefaultsOnly, Category = AttackRange)
   TScriptInterface<IAttackRangeInterface> attack_range_component_;

   UPROPERTY(VisibleAnywhere, Category = Team)
   TeamFlag team_flag_;

   bool exist_attack_taget_;

   // 임시 디버깅 심볼
   mutable FVector log_pos_;


   TWeakObjectPtr<ASquad> squad_;


   UPROPERTY(VisibleDefaultsOnly, Category = Test)
   UClass* test_billbard_generateed_class_;

   UPROPERTY(EditAnywhere, Category = Test)
   class AActor* test_billboard_;

   UnitState state_;
   float attack_range_;

   // soldier 
   float vertical_interval_;
   float horizontal_interval_;
   AAIController* controller_;

   float attack_update_delta_time_;

 public:

   //squad soldier idx number
   UPROPERTY(BlueprintReadOnly, Category = Test)
   int soldier_num_;

   FString static_mesh_file_path_;
   FString animation_blueprint_file_path_;
   FString behavior_tree_file_path_;
   FString weapon_socket_name_;

};
