// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UCurveTable;

UENUM(BlueprintType)
enum class ECharacterClass : uint8 {
    Elementalist,
    Warrior,
    Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo {
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Class Default")
    TSubclassOf<UGameplayEffect> PrimaryAttributesEffect;
};

/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
    TSubclassOf<UGameplayEffect> SecondaryAttributesEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
    TSubclassOf<UGameplayEffect> VitalAttributesEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Character Class Default")
    TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassMap;

    UPROPERTY(EditDefaultsOnly, Category = "Character Class Default | Damage")
    TObjectPtr<UCurveTable> DamageCalcCoefficientsCT;

    FCharacterClassDefaultInfo& GetClassDefaultInfo(ECharacterClass Key);
};
