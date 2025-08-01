// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"

constexpr uint8 CUSTOM_DEPTH_RED = 250;
constexpr ECollisionChannel ProjectileChannel { ECC_GameTraceChannel1 };
constexpr ECollisionChannel LootChannel { ECC_GameTraceChannel2 };

#define DELAY(time, block)\
{\
FTimerHandle TimerHandle;\
GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()block, time, false);\
}
