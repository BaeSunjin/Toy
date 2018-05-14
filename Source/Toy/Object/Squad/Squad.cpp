// Fill out your copyright notice in the Description page of Project Settings.

#include "Squad.h"
#include "Common/PS_Utils.h"
#include "Object/Unit/Character/DefaultUnit.h"
#include "Runtime/Engine/Classes/Engine/World.h"

#define REARRAGNE_DEGREE 0.7071f


struct FSortByPosition
{
  //squad move의 값에 Z를 제거해야하나?
  FSortByPosition(const FVector2D& _move,
    const FVector2D& _forward,
    const FVector2D& _right)
    : squad_move_(_move), forward_(_forward), right_(_right)
  { }

public:
  /* The Location to use in our Sort comparision. */
  FVector2D squad_move_;
  FVector2D forward_;
  FVector2D right_;

  bool operator()(const TWeakObjectPtr<ADefaultUnit>& A,
    const TWeakObjectPtr<ADefaultUnit>& B) const
  {
    // algorithm  https://blog.naver.com/baehun92/221268547904


    if (A.IsValid() && B.IsValid()) {

      FVector2D a_pos = SJ_VectorUtills::ToVector2D(A.Get()->GetActorLocation());
      FVector2D b_pos = SJ_VectorUtills::ToVector2D(B.Get()->GetActorLocation());

      FVector2D first_actor_vec = squad_move_ - a_pos;
      FVector2D second_actor_vec = squad_move_ - b_pos;

      //forward Calculate
      {

        //Get forward projection vector
        FVector2D first_projection_forward =
          SJ_VectorUtills::ToProjection(forward_, first_actor_vec);

        FVector2D second_projection_forward =
          SJ_VectorUtills::ToProjection(forward_, second_actor_vec);

        float first_size = first_projection_forward.Size();
        float second_size = second_projection_forward.Size();

        float half_horizontal_interval_ =
          A.Get()->GetHirzontalInterval() / 2.0f;

        //50.0f is character half capsule component radius
        if (FMath::Abs(first_size - second_size) > half_horizontal_interval_)
        {

          return first_size < second_size;

        }
      }


      //Right Calculate
      {

        //Get Right projection vector
        FVector2D first_projection_right =
          SJ_VectorUtills::ToProjection(right_, first_actor_vec);
        FVector2D second_projection_right =
          SJ_VectorUtills::ToProjection(right_, second_actor_vec);

        float first_size = first_projection_right.Size();
        float second_size = second_projection_right.Size();

        first_projection_right.Normalize();
        second_projection_right.Normalize();

        //if same dir
        if (first_projection_right == right_)
        {
          first_size = -first_size;
        }
        if (second_projection_right == right_) {
          second_size = -second_size;
        }

        /*UE_LOG(LogTemp, Error, TEXT("First Name : %s Second Name : %s \n first size : %f second size : %f"),
          *A.Get()->GetName(), *B.Get()->GetName(), first_size, second_size);
        UE_LOG(LogTemp, Warning, TEXT("first_projection_right : %s"), *first_projection_right.ToString());*/

        return first_size < second_size;
      }
    }
    else {

      //Sort Fail
      SJ_ASSERT(false);
      return false;
    }
  }
  //이전 코드는 D Backup에 존재함
};

// Sets default values
ASquad::ASquad()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  squad_forward_ = FVector2D(1, 0);
  squad_right_ = FVector2D(0, 1);

}

void ASquad::Init(const FVector& _pos, const FVector2D& _forward)
{
  SetActorLocation(_pos);

  squad_pos_ = SJ_VectorUtills::ToVector2D(_pos);
  squad_forward_ = _forward;
  squad_right_ = SJ_RotateUtills::ToRight(_forward);

  InitUnitPos();

}

void ASquad::InitUnitPos() {


  SJ_ASSERT(units_.Num());

  float vertical_interval = 0;
  float horizontal_interval = 0;

  if (units_[0].IsValid()) {
    vertical_interval = units_[0].Get()->GetVerticalInterval();
    horizontal_interval = units_[0].Get()->GetHirzontalInterval();
  }

  //TODO 유닛이 사용 불가능할떄 간격을 가지고 오지 못한다. 처리 방법필요
  SJ_ASSERT(vertical_interval);
  SJ_ASSERT(horizontal_interval);

  for (int soldier_num = 0; soldier_num < units_.Num(); soldier_num++) {
    FVector init_pos = GetUnitGoalPos(GetActorLocation(), vertical_interval,
                                      horizontal_interval, soldier_num);

    SJ_ASSERT(units_[soldier_num].IsValid());
    auto unit_ref = units_[soldier_num].Get();
    unit_ref->SetActorLocation(init_pos);
    FVector forward = SJ_VectorUtills::ToVector3D(squad_forward_);
    unit_ref->SetActorRotation(forward.Rotation());
      
  }

}

void ASquad::MoveSquad(const FVector& _pos)
{

  SJ_ASSERT(units_.Num());

  // 이동 위치에 따라 유닛들의 위치를 재배열한다.
  squad_pos_ = GetSquadCenter();

  FVector2D move_pos_2d = SJ_VectorUtills::ToVector2D(_pos);
  FVector2D move_dir = move_pos_2d - squad_pos_;

  //이전의 squad 방향과 비교하여 재배열 할지 구분짓는다.
  bool do_rearange = CheckNeedRearrange(move_dir);

  //스쿼드 소유의 vector들 계산
  //유닛의 재배치 관련되어 사용되어진다.
  squad_forward_ = move_pos_2d - squad_pos_;
  SJ_ASSERT(SJ_VectorUtills::Normalize(squad_forward_));

  squad_right_ = SJ_RotateUtills::ToRight(squad_forward_);
  SJ_ASSERT(SJ_VectorUtills::Normalize(squad_right_));

  if (do_rearange) {
    RearrangeSquad(move_pos_2d);
  }

  UnitsMove(_pos);

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


//TODO 예외에 관하여 처리해야된다.
FVector2D ASquad::GetSquadCenter() {

  int edge_left_up = 0;
  int edge_right_up = array_vertical_ - 1;
  int edge_left_down = array_vertical_ * array_horizental_ - array_horizental_;
  int edge_right_down = array_vertical_ * array_horizental_ - 1;

  int unit_num = units_.Num();
  bool success = (edge_left_up < unit_num && edge_right_up < unit_num &&
    edge_left_down < unit_num && edge_right_down < unit_num);

  SJ_ASSERT(success);

  if (units_[edge_left_up].IsValid() && units_[edge_right_up].IsValid() &&
    units_[edge_left_down].IsValid() && units_[edge_right_down].IsValid()) {

    FVector left_up_pos = units_[edge_left_up].Get()->GetActorLocation();
    FVector right_up_pos = units_[edge_right_up].Get()->GetActorLocation();
    FVector left_donw_pos = units_[edge_left_down].Get()->GetActorLocation();
    FVector right_donw_pos = units_[edge_right_down].Get()->GetActorLocation();

    FVector average = (left_up_pos + right_up_pos +
      left_donw_pos + right_donw_pos) / 4.0f;

    return SJ_VectorUtills::ToVector2D(average);;

  }

  SJ_ASSERT(false);
  return FVector2D(0, 0);

}

//유닛 하나하나에게 이동할 위치를 알려준다.
void ASquad::UnitsMove(const FVector& _pos)
{
  SJ_ASSERT(units_.Num());

  float vertical_interval = 0;
  float horizontal_interval = 0;

  if (units_[0].IsValid()) {
    vertical_interval = units_[0].Get()->GetVerticalInterval();
    horizontal_interval = units_[0].Get()->GetHirzontalInterval();
  }

  //TODO 유닛이 사용 불가능할떄 간격을 가지고 오지 못한다. 처리 방법필요
  SJ_ASSERT(vertical_interval);
  SJ_ASSERT(horizontal_interval);

  for (int soldier_num = 0; soldier_num < units_.Num(); soldier_num++) {
    FVector goal_pos = GetUnitGoalPos(_pos, vertical_interval,
                                        horizontal_interval, soldier_num);
    
    SJ_ASSERT(units_[soldier_num].IsValid());
    auto unit_ref = units_[soldier_num].Get();
    unit_ref->MoveTo(goal_pos, SJ_VectorUtills::ToVector3D(squad_forward_));
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

bool ASquad::CheckNeedRearrange(const FVector2D& _move_dir) {


  float dot_scala = FVector2D::DotProduct(squad_forward_, _move_dir);

  //REARRAGNE_DEGREE = 45 dgree
  return dot_scala > REARRAGNE_DEGREE;

}

void ASquad::RearrangeSquad(const FVector2D& _pos) {

  //배열의 순서가 유닛 나열의 순서
  units_.HeapSort(FSortByPosition(_pos, squad_forward_, squad_right_));

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

}

