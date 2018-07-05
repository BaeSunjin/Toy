// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitBase.h"
#include "Common/PS_Utils.h"
#include "Object/Unit/Component/AttackRange/AttackRangeInterface.h"
#include "Object/Unit/Component/Weapon/WeaponComponentBase.h"

#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

// Sets default values
AUnitBase::AUnitBase(const FObjectInitializer& ObjectInitializer)
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

}


void AUnitBase::Init(UWeaponComponentBase* _weapon,
                     IAttackRangeInterface* _attack_range)
{

  SJ_ASSERT(_weapon);
  SJ_ASSERT(_attack_range);

  // ------------colision init --------------
  InitCollision();

  // ---- heigh light mesh init ----
  InitHLMesh();

  //---- mesh init ----
  InitMesh();

  //---- controller init ----
  InitController();

  //---- animation init ----
  InitAnimBlueprint();

  //---- units interval init ----
  InitInterval(200.0f, 200.0f);

  //---- behavior tree init ----
  InitBehaviorTree();

  //---- attack range component init ----
  SetAttackRangeComponent(_attack_range, attack_range_);

  //---- weapone init ----
  SetWeapon(_weapon, unit_mesh_, weapon_socket_name_);

#if WITH_EDITOR
  //---- Helper UI init ----
  HelperUIInit();

#endif

}

// Called when the game starts or when spawned
void AUnitBase::BeginPlay()
{
  Super::BeginPlay();

  controller_ = Cast<AAIController>(GetController());

  //TODO 삭제 부분 필요.
  // setting test billbard
  test_billboard_ = GetWorld()->SpawnActor(test_billbard_generateed_class_);
  test_billboard_->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
  test_billboard_->SetActorRelativeLocation(FVector(0.0f, 0.0f, 120.0f));

}



void AUnitBase::RunAway() {

  state_.run_away_.SetData(true);

}

void AUnitBase::WorkAttackComponent() {
  attack_range_component_->Work();
}


const TWeakObjectPtr<ASquad>& AUnitBase::GetSquad() {

  return squad_;

}

void AUnitBase::SetSquad(const TWeakObjectPtr<ASquad>& data) {
  squad_ = data;
}
void AUnitBase::SetTeamFlag(const TeamFlag& _team_flag) {

  team_flag_ = _team_flag;
  attack_range_component_->SetTeamFlag(team_flag_);
}


UBlackboardData* AUnitBase::GetUnitBlackBoardData() {

  SJ_ASSERT(behavior_);
  SJ_ASSERT(behavior_->BlackboardAsset);

  return behavior_->BlackboardAsset;
}

UBehaviorTree* AUnitBase::GetUnitBehavior() {

  SJ_ASSERT(behavior_);
  return behavior_;
}

void AUnitBase::SetHighLight(bool _light_on) {

  //UE_LOG(LogTemp, Error, TEXT("On Unit HighLight"));
  high_light_component_->SetVisibility(_light_on);

}

float AUnitBase::GetHirzontalInterval() {
  return horizontal_interval_;
}

float AUnitBase::GetVerticalInterval() {
  return vertical_interval_;
}

void AUnitBase::InitCollision() {

  FCollisionResponseContainer collision_response;
  collision_response.SetResponse(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
  collision_response.SetResponse(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
  collision_response.SetResponse(ECollisionChannel::ECC_MouseRay, ECollisionResponse::ECR_Block);
  collision_response.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
  collision_response.SetResponse(ECollisionChannel::ECC_Trigger, ECollisionResponse::ECR_Overlap);

  GetCapsuleComponent()->SetCollisionResponseToChannels(collision_response);

}

void AUnitBase::InitHLMesh() {

  high_light_component_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighLight_Mesh"));
  high_light_component_->SetVisibility(false);
  high_light_component_->SetupAttachment(RootComponent);
  high_light_component_->RelativeLocation = FVector(0, 0, -85.0f);
  high_light_component_->SetCollisionEnabled(ECollisionEnabled::NoCollision);


  static ConstructorHelpers::FObjectFinder<UStaticMesh> HL_Mesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
  SJ_ASSERT(HL_Mesh.Object) {
    high_light_component_->SetStaticMesh(HL_Mesh.Object);
  }

  static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Characters/Materials/Unit_HighLight.Unit_HighLight'"));

  if (Material.Succeeded())
  {
    //auto instnace_mat = UMaterialInstanceDynamic::Create(Material.Object, this);
    high_light_component_->SetMaterial(0, Material.Object);
  }

}

void AUnitBase::InitMesh() {

  if (static_mesh_file_path_.Len() == 0) {

    UE_LOG(LogTemp, Error,
      TEXT("%s initfaile not setting static mesh file path"), *GetName());
    SJ_ASSERT(false);
  }

  static ConstructorHelpers::FObjectFinder<USkeletalMesh>
    unit_mesh(*static_mesh_file_path_);

  SJ_ASSERT(unit_mesh.Object);

  unit_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Unit_Mesh"));
  unit_mesh_->SetSkeletalMesh(unit_mesh.Object);
  unit_mesh_->SetupAttachment(RootComponent);
  unit_mesh_->RelativeLocation = FVector(0.0f, 0.0f, -90.0f);
  unit_mesh_->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
  unit_mesh_->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AUnitBase::SetWeapon(UWeaponComponentBase* _new_weapon,
                          USkeletalMeshComponent* _mesh,
                          FString _socket_name) {

  SJ_ASSERT(_mesh);
  SJ_ASSERT(_new_weapon);
  if (_socket_name.Len() == 0) {

    UE_LOG(LogTemp, Error,
      TEXT("%s initfaile not setting weapon socket name"), *GetName());
    SJ_ASSERT(false);

  }

  weapon_component_ = _new_weapon;
  weapon_component_->AttachTo(_mesh, *_socket_name);

}

void AUnitBase::InitController() {

  if (AIControllerClass == nullptr) {

    UE_LOG(LogTemp, Error,
      TEXT("%s initfaile not setting AIController class"), *GetName());

    SJ_ASSERT(false);
  }

  AutoPossessAI = EAutoPossessAI::Spawned;

}

void AUnitBase::InitAnimBlueprint() {

  SJ_ASSERT(unit_mesh_);

  if (animation_blueprint_file_path_.Len() == 0) {

    UE_LOG(LogTemp, Error,
      TEXT("%s initfaile not setting animation blueprint file path"),
      *GetName());

    SJ_ASSERT(false);
  }


  static ConstructorHelpers::FObjectFinder<UAnimBlueprint>
    anim_blueprint(*animation_blueprint_file_path_);

  SJ_ASSERT(anim_blueprint.Object);
  unit_mesh_->SetAnimInstanceClass(anim_blueprint.Object->GeneratedClass);

}

void AUnitBase::InitInterval(float _vertical, float _horizontal) {

  vertical_interval_ = _vertical;
  horizontal_interval_ = _horizontal;

}

void AUnitBase::InitBehaviorTree() {

  if (behavior_tree_file_path_.Len() == 0) {

    UE_LOG(LogTemp, Error,
      TEXT("%s initfaile not setting behavior tree file path"), *GetName());

    SJ_ASSERT(false);
  }

  static ConstructorHelpers::FObjectFinder<UBehaviorTree>
    behavior_tree(*behavior_tree_file_path_);

  if (behavior_tree.Succeeded()) {
    behavior_ = behavior_tree.Object;
  }

}

void AUnitBase::InitState() {

  state_.run_away_.SetData(false);
  state_.hp_.SetData(100);
  //TODO 나머지값.. 처리 필요.

}

void AUnitBase::SetAttackRangeComponent(IAttackRangeInterface* _new_attack_range,
  float _attack_range)
{

  SJ_ASSERT(_new_attack_range);

  if (attack_range_ <= 0) {

    UE_LOG(LogTemp, Error,
      TEXT("%s initfaile not setting attack_range_"), *GetName());

    SJ_ASSERT(false);

  }

  attack_range_component_ = _new_attack_range->_getUObject();
  attack_range_component_->Attach(RootComponent);


  attack_range_ = _attack_range;
  attack_range_component_->SetAttackRange(attack_range_);

}

void AUnitBase::OnDamage(int _damega) {

  SJ_ASSERT(_damega);

  int currnet_hp = state_.hp_.GetValue();
  currnet_hp -= _damega;

  if (currnet_hp <= 0) {
    //GetSquad()->RemoveUnit(this);
    Destroy();
  }
}

TeamFlag AUnitBase::GetTeamFlag() {
  return team_flag_;
}

#if WITH_EDITOR

void AUnitBase::HelperUIInit() {

  static ConstructorHelpers::FObjectFinder<UBlueprint> Test_Billboard(TEXT("Blueprint'/Game/UI/WUI/Solider_Idx_.Solider_Idx_'"));
  if (Test_Billboard.Succeeded())
  {
    test_billbard_generateed_class_ = Test_Billboard.Object->GeneratedClass;
  }

}
#endif

void AUnitBase::MoveTo(const FVector& _move_pos,
  const FVector& _normal_look_at,
  const bool& _retreat)
{

  SJ_ASSERT(_normal_look_at.IsNormalized());

  state_.retreat_.SetData(_retreat);
  state_.move_to_.SetData(_move_pos);
  state_.look_at_.SetData(_normal_look_at);

  log_pos_ = _move_pos;
}

const UnitState& AUnitBase::GetState() {
  return state_;
}


void AUnitBase::SetAttackTarget(ASquad* _target) {

  SJ_ASSERT(_target);

  state_.attack_target_.SetData(_target);


}


// Called every frame
void AUnitBase::Tick(float _delta_time)
{
  Super::Tick(_delta_time);
  DrawDebugLine(GetWorld(), log_pos_, log_pos_ + FVector(0, 0, 100.f), FColor::Red);

  //주변에 공격확인
  //공격 에니메이션 실행
  //에니메이션 콜백에서 Attack함수 실행
  attack_update_delta_time_ += _delta_time;
  if (attack_update_delta_time_ > ATTACK_TARGET_UPDATE_TIME) {

    GetAttackTarget();
    attack_update_delta_time_ = 0;

  }

}

bool AUnitBase::ExistMovePath() {

  SJ_ASSERT(controller_);
  auto state = controller_->GetPathFollowingComponent()->GetStatus();

  if (state == EPathFollowingStatus::Moving) { return true; }
  return false;

}



void AUnitBase::Attack() {

  auto target = GetAttackTarget();
  if (target == nullptr) { return; }

  weapon_component_->Attack(target);

}

//공격 애니메이션이 실행될떄 바로 적을 처다본다.
void AUnitBase::LookAt() {

  auto target = GetAttackTarget();
  if (target == nullptr) { return; }

  if (target) {
    auto dir = target->GetActorLocation() - GetActorLocation();
    FRotator lookAtRotator = FRotationMatrix::MakeFromX(dir).Rotator();
    SetActorRotation(lookAtRotator);
  }

}

bool AUnitBase::IsExistAttackTarget() {
  return exist_attack_taget_;
}

AUnitBase* AUnitBase::GetAttackTarget() {

  SJ_ASSERT(attack_range_component_);
  AUnitBase* target = attack_range_component_->GetAttackTarget();
  if (target) {
    exist_attack_taget_ = true;
  }
  else {
    exist_attack_taget_ = false;
  }

  return target;
}


void AUnitBase::SetAttackRange(float _attack_range) {
  attack_range_ = _attack_range;
}
