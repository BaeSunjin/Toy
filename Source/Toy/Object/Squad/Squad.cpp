// Fill out your copyright notice in the Description page of Project Settings.

#include "Squad.h"
#include "Common/PS_Utils.h"
#include "Object/Unit/Character/DefaultUnit.h"
#include "Runtime/Core/Public/Math/NumericLimits.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/World.h"



#define REARRAGNE_DEGREE 0.7071f
#define MIN_SQUAD_UNIT 7
#define MoveAttackDistance 2
#define WaitAttackDistnace 1

const static float POSITION_UPDATE_TIME = 1.0f;

// Sets default values
ASquad::ASquad()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  root_component_ = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
  root_component_->SetRelativeLocation(FVector(0, 0, 0));
  SetRootComponent(root_component_);

  squad_forward_ = FVector2D(1, 0);
  squad_right_ = FVector2D(0, 1);

  squad_behavior_state_ = FSquadBehaviorState::kWait;
  retreat_ = false;
  controlling_ = false;

  //
  
  collsion_component_ = 
    CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
  
  collsion_component_->AttachToComponent(RootComponent,
    FAttachmentTransformRules::KeepRelativeTransform);
  
  collsion_component_->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
  collsion_component_->SetSphereRadius(32.0f);
  collsion_component_->
    SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

  collsion_component_->SetCollisionObjectType(ECollisionChannel::ECC_Squad);

  

}

void ASquad::Init(const FVector& _pos, const FVector2D& _forward,
                  const TeamFlag& _team_flag)
{
  SetActorLocation(_pos);

  squad_pos_ = SJ_VectorUtills::ToVector2D(_pos);
  squad_forward_ = _forward;
  squad_right_ = SJ_RotateUtills::ToRight(_forward);

  InitUnitTransform();

  team_flag_ = _team_flag;

  collsion_component_->
    SetCollisionResponseToChannel(ECollisionChannel::ECC_Trigger,
      ECollisionResponse::ECR_Overlap);

}

void ASquad::InitUnitTransform() {

  SJ_ASSERT(units_.Num());

  if (units_[0].IsValid()) {
    unit_vertical_interval_ = units_[0].Get()->GetVerticalInterval();
    unit_hoizental_interval_ = units_[0].Get()->GetHirzontalInterval();
  }

  //TODO 유닛이 사용 불가능할떄 간격을 가지고 오지 못한다. 처리 방법필요
  SJ_ASSERT(unit_vertical_interval_);
  SJ_ASSERT(unit_hoizental_interval_);

  for (int soldier_num = 0; soldier_num < units_.Num(); soldier_num++) {

    FVector init_pos = GetUnitGoalPos(GetActorLocation(), unit_vertical_interval_,
                                      unit_vertical_interval_, soldier_num);

    SJ_ASSERT(units_[soldier_num].IsValid());

    // position init
    auto unit_ref = units_[soldier_num].Get();
    unit_ref->SetActorLocation(init_pos);

    // rotation init
    FVector forward = SJ_VectorUtills::ToVector3D(squad_forward_);
    unit_ref->SetActorRotation(forward.Rotation());
      
  }

}

void ASquad::MoveSquad(const FVector& _pos){

  SJ_ASSERT(units_.Num());

  FVector2D move_pos_2d = SJ_VectorUtills::ToVector2D(_pos);
  FVector2D move_dir = move_pos_2d - squad_pos_;
  SJ_ASSERT(SJ_VectorUtills::Normalize(move_dir));

  //이전의 squad 방향과 비교하여 재배열 할지 구분짓는다.
  bool do_rearange = CheckNeedRearrange(move_dir);

  //스쿼드 소유의 vector들 계산
  //유닛의 재배치 관련되어 사용되어진다.
  squad_forward_ = move_pos_2d - squad_pos_;
  SJ_ASSERT(SJ_VectorUtills::Normalize(squad_forward_));

  squad_right_ = SJ_RotateUtills::ToRight(squad_forward_);
  SJ_ASSERT(SJ_VectorUtills::Normalize(squad_right_));

  //TODO 2D 벡터로 변경해야하나 고민필요.
  TArray<FVector> goals;
  SJ_ASSERT(MakeGoals(_pos, goals));

  if (do_rearange) {
    RearrangeSquad(goals);
  }

  UnitsMove(goals, retreat_);
  SetActorLocation(_pos);

}

void ASquad::SetHighLight(bool _light_on)
{
  SJ_ASSERT(units_.Num())

    for (auto& unit : units_) {
      if (unit.IsValid()) {
        unit->SetHighLight(_light_on);
      }
    }
}

//유닛 하나하나에게 이동할 위치를 알려준다.
void ASquad::UnitsMove(const TArray<FVector>& _goals, 
                        const bool& _retreat)
{
 
  SJ_ASSERT(_goals.Num());
  SJ_ASSERT(units_.Num());
  SJ_ASSERT(_goals.Num() == units_.Num());

  FVector forward = SJ_VectorUtills::ToVector3D(squad_forward_);
  SJ_ASSERT(forward.Normalize());

  int num = _goals.Num();
  for (int i = 0; i < num; i++) {

    SJ_ASSERT(units_[i].IsValid());
    auto unit = units_[i].Get();
    unit->MoveTo(_goals[i], forward, _retreat);
    
  }
}


FVector ASquad::GetUnitGoalPos(const FVector& _move_pos,
                               const float& _vertical_interval,
                               const float& _horziontal_interval_,
                               const int& _soldier_num)
{

  SJ_ASSERT(array_vertical_);
  float harf_vertical = array_vertical_ / 2.0f;

  FVector goal_pos = _move_pos;

  //각 유닛의 최종 목적지 계산 및 이동 명령
  SJ_ASSERT(units_.Num());
  auto unit = units_[_soldier_num];

  SJ_ASSERT(unit.IsValid());

  auto unit_ref = unit.Get();
  unit_ref->soldier_num_ = _soldier_num;

  //유닛이 움직여야할 위치 인덱스 계산
  float vertical_pos_idx = (_soldier_num % array_vertical_) - harf_vertical;
  float horizontal_pos_idx = (_soldier_num / array_vertical_);

  // vertial position correction from sqaud
  FVector added_right = SJ_VectorUtills::ToVector3D(squad_right_);
  added_right *= vertical_pos_idx * _vertical_interval;
  goal_pos += added_right;

  //horizontal position correction from squad
  FVector added_forward = SJ_VectorUtills::ToVector3D(squad_forward_);
  added_forward *= -horizontal_pos_idx * _horziontal_interval_;
  goal_pos += added_forward;

  return goal_pos;

}

bool ASquad::MakeGoals(const FVector& _move_pos, TArray<FVector>& _out_goals)
{

  int unit_num = units_.Num();
  SJ_ASSERT(unit_num);

  for (int soldier_num = 0; soldier_num < unit_num; soldier_num++) {
    
    FVector GoalPos = GetUnitGoalPos(_move_pos, unit_vertical_interval_,
                                     unit_hoizental_interval_, soldier_num);
    
    _out_goals.Add(GoalPos);
  }

  if (_out_goals.Num() != unit_num) { return false; }
  return true;
  
}

bool ASquad::CheckNeedRearrange(const FVector2D& _move_dir) {

  SJ_ASSERT(SJ_VectorUtills::IsNormalized(_move_dir));

  float dot_scala = FVector2D::DotProduct(squad_forward_, _move_dir);

  //REARRAGNE_DEGREE = 45 dgree
  return dot_scala < REARRAGNE_DEGREE;

}

void ASquad::RearrangeSquad(const TArray<FVector>& goals)
{
  int goal_num = goals.Num();
  int unit_num = units_.Num();
  SJ_ASSERT(goal_num);
  SJ_ASSERT(unit_num);
  SJ_ASSERT(goal_num == unit_num);

  // 의도하는 동작은 goals의 각 요소의 idx와
  // units_에서의 가장 짧은 거리를 가진 유닛의 idx를
  // 같게 만들어 준다.


  int array_num = goal_num;
  for (int goal_idx = 0 ; goal_idx < (goal_num - 1) ; goal_idx++) {

    int near_unit_idx = goal_idx;
    float length = TNumericLimits< float >::Max();

    for (int unit_idx = goal_idx; unit_idx < array_num; unit_idx++) {

      SJ_ASSERT(units_[unit_idx].IsValid());

      auto unit = units_[unit_idx].Get();
      FVector unit_pos = unit->GetActorLocation();
      float new_length = (goals[goal_idx] - unit_pos).SizeSquared();
      
      if (new_length < length) {
        length = new_length;
        near_unit_idx = unit_idx;
      }
    }


    // 유닛의 위치 변경 필요 x
    if (near_unit_idx == goal_idx) { continue; }

    // 유닛의 위치를 변경한다.    
    units_.Swap(goal_idx, near_unit_idx);

  }



  //유닛에 숫자부여
  //추후에 사용가능성 많음..
  int count = 0;
  for (auto unit : units_) {
    SJ_ASSERT(unit.IsValid());
    unit.Get()->soldier_num_ = count;
    count++;
  }

}

void ASquad::ClearSquad() {

  for (auto& unit : units_) {
    if (unit.IsValid()) {
      //Behavior Tree will do - run away and destroy
      unit.Get()->RunAway();
      
    }
  }

  //clear
  units_.Empty();

}

// Called when the game starts or when spawned
void ASquad::BeginPlay()
{
  Super::BeginPlay();

}

// Called every frame
void ASquad::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (units_.Num() < MIN_SQUAD_UNIT) {

    //squad 제거
    //player controller에 문제가 생길수도 있다.
    ClearSquad();
    Destroy();
    return;
  }

  if (!controlling_) {
    if (units_[0].IsValid()) {
      if (units_[0].Get()->ExistMovePath()) {
        SetRetreat(false);
      }
    }
  }

  MoveUpdate(DeltaTime);

}

void ASquad::SetAttackTargetToUnits(ASquad* _squad) {

  SJ_ASSERT(_squad);

  for (auto& unit : units_) {
    if (unit.IsValid()) {
      unit.Get()->SetAttackTarget(_squad);
    }

  }
}

void ASquad::SetControlling(bool _controlling) {
  controlling_ = _controlling;
}


const TeamFlag& ASquad::GetTeamFlag() {
  return team_flag_;
}


void ASquad::MoveUpdate(float _delata) {

  position_update_timer_ += _delata;
  if (position_update_timer_ < POSITION_UPDATE_TIME) { return; }
  else { position_update_timer_ = 0.0f; }

  int unit_num = units_.Num();
  int half = unit_num / 2;

  if (half == 0) { 
    UE_LOG(LogTemp, Error, TEXT("Squad Move Update someting wrong"));
    return;
  }

  if (!units_[half].IsValid()) { return; }
  
  SetActorLocation(
    units_[half].Get()->GetActorLocation());

}

void ASquad::SetRetreat(const bool& _retreat) {
  retreat_ = _retreat;
}


bool ASquad::GetRetreat() {
  return retreat_;
}

void ASquad::RemoveUnit(ADefaultUnit* _unit) {

  SJ_ASSERT(_unit);

  bool find = false;
  int count = units_.Num();
  int array_idx;
  for (array_idx = 0; array_idx < count; ++array_idx) {
    if (units_[array_idx].IsValid()) {
      auto unit = units_[array_idx].Get();
      if (unit->GetSerialNum() == _unit->GetSerialNum()) {
        find = true;
        break;
      }
    }
  }

  if (find) {
    units_.RemoveAt(array_idx);
  }
}