// Fill out your copyright notice in the Description page of Project Settings.

#include "Senario.h"
#include "Common/PS_Utils.h"
#include "Object/Tool/SquadMaker.h"
#include "Object/Unit/Character/UnitTypesInfo.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"



class SenarioDataLoader {

private:

  SenarioDataLoader() = delete;

public:

  static void Load(const int& _stage, FSenarioData** _out_data) {
    
    auto table_asset = LoadObject<UDataTable>(NULL,
      TEXT("DataTable'/Game/Data/Senario/Senario.Senario'"));

    SJ_ASSERT(table_asset);
   
    //key
    auto stage_name = FString::Printf(TEXT("%d"), _stage);
    FName raw_name(*stage_name);

    // ���� �߻��� ��� �޼��� ǥ��
    static const FString context_string("GENERAL");

    *_out_data = table_asset->FindRow<FSenarioData>(raw_name, context_string);
    SJ_ASSERT(_out_data);
    
  }
};


void Senario::GetSenarioData(const int& _stage,
                             TArray<FSquadUnitsInfo>& _out_info)
{

  //�ó����� ���� �ε� 
  FSenarioData* out;
  SenarioDataLoader::Load(_stage, &out);

  //TODO squad���� �ε� �ϴ� �κ����� �����ؾߵ�
  int32 unit_num = out->unit_num_;
  
  if (unit_num <= 1) {
    //TODO ������ ���ڸ� �˷��ִ� �κ��� �ۼ��ؾߵ�
    int num = 10;
    FSquadUnitsInfo squad_unit_info(num,
                                  static_cast<EPlayerRace>(out->unit_race1_));

    _out_info.Add(squad_unit_info);
  }
  
}
