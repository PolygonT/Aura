// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FDefaultAttributeInfo {
    GENERATED_BODY()

    FDefaultAttributeInfo() = default;
    FDefaultAttributeInfo(FGameplayTag AttributeTag, FText AttributeName,
                          FText AttributeDescription);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag AttributeTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText AttributeName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText AttributeDescription;

    UPROPERTY(BlueprintReadOnly)
    float AttributeValue { 0.f };
};

/**
 * 
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UAttributeInfo();

    FDefaultAttributeInfo*
    FindAttributeInfoByTag(const FGameplayTag &AttributeTag,
                           bool bLogNotFound);

    void InitAttributeInfoMap();

    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TMap<FGameplayTag, FDefaultAttributeInfo> AttributeInfoMap;

};
