// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/SerialNum.h"
#include "Rule/Rules.h"
#include "GameFramework/Actor.h"
#include "Squad.generated.h"

class AUnitBase;
class USphereComponent;


enum class FSquadBehaviorState {

  kMove = 0,
  kMoveAndFight,
  kWait

};

//struct�� ���� �� squad manager�� �����Ǿ���� ������ �����صд�.
USTRUCT(BlueprintType)
struct FSquadInfo {

  GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int32 org_unit_count_;

  UPROPERTY(VisibleAnywhere)
	int32 max_unit_count_;

  UPROPERTY(VisibleAnywhere)
	int32 min_unit_count_;

};


UCLASS()
class TOY_API ASquad : public AActor, public SerialNum<ASquad>
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASquad();

  void Init(const FVector& _pos, const FVector2D& _forward,
    const TeamFlag& _team_flag);
  void InitUnitTransform();

  // 2D�� ���ڸ� �޴� ������ �������� ������ó���� xy ��ǥ�θ� �����ϱ� ����
  void MoveSquad(const FVector& _pos);
  void SetHighLight(bool _light_on);
  const TeamFlag& GetTeamFlag();

  void RemoveUnit(AUnitBase* _unit);

private:

  void UnitsMove(const TArray<FVector>& _goals, const bool& _attack);
  FVector GetUnitGoalPos(const FVector& _move_pos,
                         const float& _vertical_interval,
                         const float& _horziontal_interval_,
                         const int& _soldier_num);

  bool MakeGoals(const FVector& _move_pos, TArray<FVector>& _out_goals);

  bool CheckNeedRearrange(const FVector2D& _move_dir);
  void MoveUpdate(float _delata);

  //�뿭 �ʱ�ȭ
  void RearrangeSquad(const TArray<FVector>& goals);
  void ClearSquad();
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void SetAttackTargetToUnits(ASquad* _squad);

  // Called every frame
  virtual void Tick(float DeltaTime) override;


public:

  TArray<TWeakObjectPtr<AUnitBase>> units_;
  FSquadInfo squad_info_;
  FSquadBehaviorState squad_behavior_state_;
  
  int currnet_unit_count_;

  int array_vertical_;
  int array_horizental_;

  float unit_vertical_interval_;
  float unit_hoizental_interval_;

  UFUNCTION(BlueprintCallable, Category = "Squad")
  void SetRetreat(const bool& _retreat);

  UFUNCTION(BlueprintPure, Category = "Squad")
  bool GetRetreat();

  void SetControlling(bool _controlling);

 private:

   bool retreat_;
   bool controlling_;

   FVector2D squad_forward_;
   FVector2D squad_right_;

   FVector2D squad_pos_;
   TeamFlag team_flag_;

   UPROPERTY(VisibleDefaultsOnly)
   USceneComponent* root_component_;

   UPROPERTY(VisibleDefaultsOnly)
   USphereComponent* collsion_component_;

   float position_update_timer_;

};

// ��� �����ؾ� �ϴ� �ɱ�?

// ������ Ű�� �޸��� �Ѵ�
// �׷��� ��� �����ϰ�..;;

// Squad���� ������ ������ �ִ°� �´�.
// Ư�� �������� �����ϸ� �ٽ� Ǯ���ְ�
