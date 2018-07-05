// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define ECC_MouseRay ECC_GameTraceChannel1
#define ECC_Squad ECC_GameTraceChannel2
#define ECC_Trigger ECC_GameTraceChannel3


namespace SJ_RotateUtills {

  FVector ToRight(const FVector& _forward);
  FVector2D ToRight(const FVector2D& _forward);

}


namespace SJ_VectorUtills{

  // z value ignore
  FVector2D ToVector2D(const FVector& _vec3);

  // z value is setting zero
  FVector ToVector3D(const FVector2D& _vec2);

  // FVector2D Proejction
  FVector2D ToProjection(const FVector2D& _pivot,
                         const FVector2D& _projection_vec);

  
  bool Normalize(FVector2D& _vec);
  bool IsNormalized(const FVector2D& _vec);

}


void Shutdown(const TCHAR* _file, const int& _line, const TCHAR* _function);
void PrintLog(const TCHAR* _file, const int& _line, const TCHAR* _function);
void QuitTheGame();


#define SJ_ASSERT(condition)\
  if (!condition) {\
	  Shutdown(UTF8_TO_TCHAR(__FILE__), __LINE__, UTF8_TO_TCHAR(__func__));\
  }



