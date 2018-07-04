// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/SerialNum.h"
#include "GameFramework/Character.h"
#include "Object/Tool/SquadMaker.h"
#include "Runtime/Core/Public/Containers/Map.h"
#include "DefaultUnit.generated.h"

class AAIController;
class ASquad;
class UBoxComponent;
class UBlackboardData;
class UBehaviorTree;
class UCapsuleComponent;
class IAttackRangeInterface;
class UWeaponComponentBase;


USTRUCT(BlueprintType)
struct FUnitState {

  GENERATED_USTRUCT_BODY()

public:

  bool changed_;

  
  // first is value second bool change flag
  TTuple<bool, bool> retreat_;
  TTuple<ASquad*, bool> attack_target_;
  TTuple<bool,bool> run_away_;
  TTuple<int, bool> hp_;
  TTuple<FVector, bool> move_to_;
  TTuple<FVector, bool> look_at_;
  
};

UCLASS()
class TOY_API ADefaultUnit : public ACharacter, public SerialNum<ADefaultUnit>
{
  GENERATED_BODY()

 public:
   // Sets default values for this character's properties
   ADefaultUnit();

   // call all init function
   void Init(UWeaponComponentBase* _weapon,
             IAttackRangeInterface* _attack_range);

   const TWeakObjectPtr<ASquad>& GetSquad();
   float GetHirzontalInterval();
   float GetVerticalInterval();
   ADefaultUnit* GetAttackTarget();
   TeamFlag GetTeamFlag();
   bool ExistMovePath();
   
   UFUNCTION(BlueprintPure, Category = "AttackTarget")
   bool IsExistAttackTarget();

   void SetHighLight(bool _light_on);
   void SetAttackTarget(ASquad* _target);
   void SetAttackRange(float _attack_range);
   

   void RunAway();
   void OnDamage(int _damega);
   void MoveTo(const FVector& _move_pos,
               const FVector& _normal_look_at,
               const bool& _retreat);

   // Called every frame
   virtual void Tick(float _delta_time) override;

   //TODO �߰�.. ���� �ʿ�..;; ���� ���� �������� ���� ���־����.
   void WorkAttackComponent();
   
   UFUNCTION(BlueprintCallable, Category = "AttackTarget")
   void Attack();

   UFUNCTION(BlueprintCallable, Category = "AttackTarget")
   void LookAt();

  

  
  
  
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

   //SquadMaker������ ����� �Ѵ�.
   friend class SquadUnitMaker;
   void SetSquad(const TWeakObjectPtr<ASquad>& data);
   void SetTeamFlag(const TeamFlag& _team_flag);


   //ADefaultUnitController ������ ���� ���
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

   // �ӽ� ����� �ɺ�
   mutable FVector log_pos_;


   TWeakObjectPtr<ASquad> squad_;


   UPROPERTY(VisibleDefaultsOnly, Category = Test)
   UClass* test_billbard_generateed_class_;

   UPROPERTY(EditAnywhere, Category = Test)
   class AActor* test_billboard_;

   FUnitState state_;
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

