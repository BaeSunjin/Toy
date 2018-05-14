// Fill out your copyright notice in the Description page of Project Settings.

#include "PS_Utils.h"
#include "Engine.h"

// 항상 해결해야하는 치명적인 오류에 대한 로깅 
DECLARE_LOG_CATEGORY_EXTERN(AssertError, Log, All);
DEFINE_LOG_CATEGORY(AssertError);

using namespace std;

FVector SJ_RotateUtills::ToRight(const FVector& _forward) {

  SJ_ASSERT(_forward.IsNormalized());
  static const FRotator rotate_yaw = FRotator(0.0f, 90.0f, 0.0f);
  return rotate_yaw.RotateVector(_forward);

}

FVector2D SJ_RotateUtills::ToRight(const FVector2D& _forward) {

  SJ_ASSERT(_forward.Size() == 1.0f);
  static const float rotate_angle = 90.0f;

  float sin_value = FMath::Sin(rotate_angle);
  float cos_value = FMath::Cos(rotate_angle);

  // rotate point
  float x_new = _forward.X * cos_value - _forward.Y * sin_value;
  float y_new = _forward.X * sin_value + _forward.Y * cos_value;

  return FVector2D(x_new, y_new);

}

FVector2D SJ_VectorUtills::ToVector2D(const FVector& _vec3) {

  // z value ingnore
  // Z값이 필요없는 곳에서 만 사용되어야 한다.
  FVector2D ret(_vec3.X, _vec3.Y);
  return ret;
}

FVector SJ_VectorUtills::ToVector3D(const FVector2D& _vec2) {

  // z value set 0
  // Z값이 필요없는 곳에서 만 사용되어야 한다.
  FVector ret(_vec2.X, _vec2.Y, 0);
  return ret;
}

FVector2D SJ_VectorUtills::ToProjection(const FVector2D& _pivot,
                                        const FVector2D& _projection_vec)
{

  // 내적을 얻는다.
  float dot_scala = FVector2D::DotProduct(_pivot, _projection_vec);

  // 벡터의 길이를 구한다. 
  
  float pivot_size = _pivot.SizeSquared();
  float _projection_vector_size = _projection_vec.SizeSquared();
  float cos_scala = dot_scala / (pivot_size * _projection_vector_size);
  // v1P의 길이는 
  float projectioned_size = cos_scala * _projection_vector_size;

  return FVector2D(projectioned_size * _pivot.X / pivot_size,
                  (projectioned_size *  _pivot.Y / pivot_size));
   
}

bool SJ_VectorUtills::Normalize(FVector2D& _vec) {

  float size = _vec.Size();
  if (size != 0) {
    return false;
  }

  _vec = _vec / size;
  return true;
}



void Shutdown(const TCHAR*_file, const int& _line, const TCHAR* _function)
{

#if WITH_EDITOR
  PrintLog(_file, _line, _function);
  QuitTheGame();
#endif

}

void PrintLog(const TCHAR*_file, const int& _line, const TCHAR*_function)
{

  UE_LOG(AssertError, Error,
	TEXT("Assert [ File : %s Line : %d Function : %s]"),
	_file, _line, _function);

}

void QuitTheGame() {

  //check assert
  if (GWorld) {

	APlayerController *player_controller = GWorld->GetFirstPlayerController();
	check(player_controller);

	//player_controller->IsLocalController();
	player_controller->ConsoleCommand("quit");
  }

}