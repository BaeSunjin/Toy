// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultUnit.h"
#include "Common/PS_Utils.h"
#include "DrawDebugHelpers.h"
#include "Object/Squad/Squad.h"
#include "Object/Unit/Controller/DefaultUnitController.h"
#include "Object/Unit/Component/AttackRange/AttackRangeInterface.h"
#include "Object/Unit/Component/AttackRange/BoxAttackRangeComponent.h"
#include "Object/Unit/COmponent/Weapon/TwoHandedAxComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Runtime/Core/Public/Math/NumericLimits.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"


const static float ATTACK_TARGET_UPDATE_TIME = 1.0f;

// Sets default values
ADefaultUnit::ADefaultUnit()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  // init data setting
  static_mesh_file_path_ = "SkeletalMesh'/Game/Characters/Animations/TwoHandedSword_AnimsetPro/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'";
  animation_blueprint_file_path_ = "AnimBlueprint'/Game/Characters/Animations/TwoHandedSword_AnimsetPro/Animations/AnimBlueprint/2hand_sward_warrior.2hand_sward_warrior'";
  behavior_tree_file_path_ = "BehaviorTree'/Game/Characters/AI/DefulatUnit_BT.DefulatUnit_BT'";
  weapon_socket_name_ = "LeftHandSocket";
  AIControllerClass = ADefaultUnitController::StaticClass();
  SetAttackRange(125.0f);

  auto weapon_instnace =
    CreateDefaultSubobject<UTwoHandedAxComponent>(TEXT("Weapon_Component"));

  auto attack_range = CreateDefaultSubobject<UBoxAttackRangeComponent>(
    TEXT("AttackRangeComponent"));

  Init(weapon_instnace, attack_range);


  
}

void ADefaultUnit::Init(UWeaponComponentBase* _weapon,
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
void ADefaultUnit::BeginPlay()
{
  Super::BeginPlay();

  controller_ = Cast<AAIController>(GetController());

  // setting test billbard
  test_billboard_ = GetWorld()->SpawnActor(test_billbard_generateed_class_);
  test_billboard_->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
  test_billboard_->SetActorRelativeLocation(FVector(0.0f, 0.0f, 120.0f));

}



void ADefaultUnit::RunAway() {

  state_.changed_ = true;
  state_.run_away_.Key = true;
  state_.run_away_.Value = true;

  test_billboard_->Destroy();
}

void ADefaultUnit::WorkAttackComponent() {
  attack_range_component_->Work();
}


const TWeakObjectPtr<ASquad>& ADefaultUnit::GetSquad() {

  return squad_;

}

void ADefaultUnit::SetSquad(const TWeakObjectPtr<ASquad>& data) {
  squad_ = data;
}
void ADefaultUnit::SetTeamFlag(const TeamFlag& _team_flag) {

  team_flag_ = _team_flag;  
  attack_range_component_->SetTeamFlag(team_flag_);
}


UBlackboardData* ADefaultUnit::GetUnitBlackBoardData() {

  SJ_ASSERT(behavior_);
  SJ_ASSERT(behavior_->BlackboardAsset);

  return behavior_->BlackboardAsset;
}

UBehaviorTree* ADefaultUnit::GetUnitBehavior() {

  SJ_ASSERT(behavior_);
  return behavior_;
}

void ADefaultUnit::SetHighLight(bool _light_on) {

  //UE_LOG(LogTemp, Error, TEXT("On Unit HighLight"));
  high_light_component_->SetVisibility(_light_on);

}

float ADefaultUnit::GetHirzontalInterval() {
  return horizontal_interval_;
}

float ADefaultUnit::GetVerticalInterval() {
  return vertical_interval_;
}

void ADefaultUnit::InitCollision() {

  FCollisionResponseContainer collision_response;
  collision_response.SetResponse(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
  collision_response.SetResponse(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
  collision_response.SetResponse(ECollisionChannel::ECC_MouseRay, ECollisionResponse::ECR_Block);
  collision_response.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
  collision_response.SetResponse(ECollisionChannel::ECC_Trigger, ECollisionResponse::ECR_Overlap);

  GetCapsuleComponent()->SetCollisionResponseToChannels(collision_response);

}

void ADefaultUnit::InitHLMesh() {

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

void ADefaultUnit::InitMesh() {
  
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

void ADefaultUnit::SetWeapon(UWeaponComponentBase* _new_weapon,
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

void ADefaultUnit::InitController() {

  if (AIControllerClass == nullptr) {

    UE_LOG(LogTemp, Error,
      TEXT("%s initfaile not setting AIController class"), *GetName());

    SJ_ASSERT(false);
  }
  
  AutoPossessAI = EAutoPossessAI::Spawned;

}

void ADefaultUnit::InitAnimBlueprint() {

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

void ADefaultUnit::InitInterval(float _vertical, float _horizontal) {

  vertical_interval_ = _vertical;
  horizontal_interval_ = _horizontal;

}

void ADefaultUnit::InitBehaviorTree() {

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

void ADefaultUnit::InitState() {

  state_.changed_ = true;
  state_.run_away_.Key = false;
  state_.run_away_.Value = true;

  state_.hp_.Key = 100;
  state_.hp_.Value = true;

}

void ADefaultUnit::SetAttackRangeComponent(IAttackRangeInterface* _new_attack_range,
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

void ADefaultUnit::OnDamage(int _damega) {

  SJ_ASSERT(_damega);

  state_.changed_ = true;
  state_.hp_.Key -= _damega;
  state_.hp_.Value = true;

}

TeamFlag ADefaultUnit::GetTeamFlag() {
  return team_flag_;
}

#if WITH_EDITOR

void ADefaultUnit::HelperUIInit() {

  static ConstructorHelpers::FObjectFinder<UBlueprint> Test_Billboard(TEXT("Blueprint'/Game/UI/WUI/Solider_Idx_.Solider_Idx_'"));
  if (Test_Billboard.Succeeded())
  {
    test_billbard_generateed_class_ = Test_Billboard.Object->GeneratedClass;
  }

}
#endif

void ADefaultUnit::MoveTo(const FVector& _move_pos,
                          const FVector& _normal_look_at,
                          const bool& _retreat)
{

  SJ_ASSERT(_normal_look_at.IsNormalized());

  state_.changed_ = true;
  state_.retreat_.Key = _retreat;
  state_.retreat_.Value = true;
  state_.move_to_.Key = _move_pos;
  state_.move_to_.Value = true;

  state_.look_at_.Key = _normal_look_at;
  state_.look_at_.Value = true;

  log_pos_ = _move_pos;
}


void ADefaultUnit::SetAttackTarget(ASquad* _target) {

  SJ_ASSERT(_target);

  state_.changed_ = true;
  state_.attack_target_.Key = _target;
  state_.attack_target_.Value = true;

}


// Called every frame
void ADefaultUnit::Tick(float _delta_time)
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

bool ADefaultUnit::ExistMovePath() {

  SJ_ASSERT(controller_);
  auto state = controller_->GetPathFollowingComponent()->GetStatus();
  
  if (state == EPathFollowingStatus::Moving) { return true; }
  return false;

}



void ADefaultUnit::Attack() {

  auto target = GetAttackTarget();
  if (target == nullptr) { return; }

  weapon_component_->Attack(target);

}

//공격 애니메이션이 실행될떄 바로 적을 처다본다.
void ADefaultUnit::LookAt() {

  auto target = GetAttackTarget();
  if (target == nullptr) { return; }

  if (target) {
    auto dir = target->GetActorLocation() - GetActorLocation();
    FRotator lookAtRotator = FRotationMatrix::MakeFromX(dir).Rotator();
    SetActorRotation(lookAtRotator);
  }

}

bool ADefaultUnit::IsExistAttackTarget() {
  return exist_attack_taget_;
}

ADefaultUnit* ADefaultUnit::GetAttackTarget() {

  SJ_ASSERT(attack_range_component_);
  ADefaultUnit* target = attack_range_component_->GetAttackTarget();
  if (target) {
    exist_attack_taget_ = true;
  }
  else {
    exist_attack_taget_ = false;
  }
  
  return target;
}


void ADefaultUnit::SetAttackRange(float _attack_range) {
  attack_range_ = _attack_range;
}