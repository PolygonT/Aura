// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DefaultInputConfig.generated.h"

class UInputAction;

/**
 * 
 */
UCLASS()
class AURA_API UDefaultInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
  const UInputAction *
  FindAbilityInputActionByTag(const FGameplayTag &InputTag,
                              bool bLogNotFound = false) const;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TMap<FGameplayTag, UInputAction*> AbilityInputActionMap;
};
