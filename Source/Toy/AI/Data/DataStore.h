// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Unit/Controller/DefaultUnitController.h"

/**
 * 
 */
namespace Toy {

  namespace BlackBoard {

    template<typename T>
    class TOY_API DataStore
    {

    public:
      explicit DataStore(FString _name);
      ~DataStore();

      T GetValue();
      FString GetName();
      void SetData(const T& _new_value);

      bool IsChange();
      void Pushed();

    private:


      FString name_;
      T value_;
      bool changed_;

    };


    template<typename T>
    DataStore<T>::DataStore(FString _name) : name_(_name)
    {}

    template<typename T>
    DataStore<T>::~DataStore() {}

    template<typename T>
    void DataStore<T>::SetData(const T& _new_value) {
      value_ = _new_value;
      changed_ = true;
    }

    template<typename T>
    T DataStore<T>::GetValue() {
      return value_;
    }

    template<typename T>
    FString DataStore<T>::GetName() {
      return name_;
    }

    template<typename T>
    bool DataStore<T>::IsChange() {
      return changed_;
    }

    template<typename T>
    void DataStore<T>::Pushed() {
      changed_ = false;
    }

    template<typename T, typename U>
    static void PushToBlackBoard(DataStore<T>& _data,
      ADefaultUnitController& _controller) {

      if (!_data.IsChange()) { return; }

      auto onwer_blackboard =
        _controller.GetBlackBoardComponent();

      FName name = (*_data.GetName());
      onwer_blackboard->SetValue<U>(name,_data.GetValue());
      _data.Pushed();

    }
  }
}
