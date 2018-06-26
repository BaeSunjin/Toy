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
class UDefaultWeaponComponent;

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


  const TWeakObjectPtr<ASquad>& GetSquad();

  void RunAway();
  void SetHighLight(bool _light_on);
  void SetAttackTarget(ASquad* _target);
  float GetHirzontalInterval();
  float GetVerticalInterval();

  //override를 사용해서
  //자식 Unit마다 다른 정보들을 로드해서 사용한다.
  virtual void InitHLMesh();
  virtual void InitMesh();
  virtual void InitWeapon();
  virtual void InitController();
  virtual void InitAnimBlueprint();
  virtual void InitInterval();
  virtual void InitBehaviorTree();
  virtual void InitState();
  virtual void InitAttackRangeComponent();

  void WorkAttackComponent();

  void OnDamage(int _damega);
  bool ExistMovePath();

  ADefaultUnit* GetAttackTarget();

  UFUNCTION(BlueprintCallable, Category = "AttackTarget")
  void Attack();

  UFUNCTION(BlueprintPure, Category = "AttackTarget")
  bool IsExistAttackTarget();

  
  TeamFlag GetTeamFlag();
  

#if WITH_EDITOR
  virtual void HelperUIInit();
#endif

  virtual void MoveTo(const FVector& _move_pos,
                      const FVector& _normal_look_at,
                      const bool& _retreat);

  // Called every frame
  virtual void Tick(float _delta_time) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
  virtual void BeginDestroy() override;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;


private:

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

private:

  UPROPERTY(EditAnywhere, Category = HighLight)
    UStaticMeshComponent* high_light_component_;


  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    USkeletalMeshComponent* unit_mesh_;

  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    UDefaultWeaponComponent* weapon_component_;

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

};