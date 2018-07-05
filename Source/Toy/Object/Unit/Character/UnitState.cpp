// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitState.h"

UnitState::UnitState():
  retreat_("Retreat"),
  attack_target_("AttackTarget"),
  run_away_("RunAway"),
  hp_("Hp"),
  move_to_("MoveTo"),
  look_at_("LookAt")
{
}

UnitState::~UnitState()
{
}
