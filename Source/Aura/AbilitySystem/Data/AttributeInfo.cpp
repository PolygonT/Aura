// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"
#include "Containers/Map.h"
#include "DefaultGameplayTags.h"
#include "Internationalization/Text.h"

FDefaultAttributeInfo::FDefaultAttributeInfo(FGameplayTag AttributeTag,
                                             FText AttributeName,
                                             FText AttributeDescription) 
: AttributeTag(AttributeTag), AttributeName(AttributeName), AttributeDescription(AttributeDescription) {}

FDefaultAttributeInfo*
UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag &AttributeTag,
                                       bool bLogNotFound) {
    if (AttributeInfoMap.Contains(AttributeTag)) {
        return AttributeInfoMap.Find(AttributeTag);
    }

    if (bLogNotFound) {
        UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s], on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
    }

    return nullptr;
}

void UAttributeInfo::InitAttributeInfoMap() {
    for (const FGameplayTag& Tag : FDefaultGameplayTags::Get().Iter) {

        FDefaultAttributeInfo Info { 
            Tag,
            FText::FromString(Tag.ToString()),
            FText::FromName("")
        };

        AttributeInfoMap.Add(Tag, Info);
    }
}

UAttributeInfo::UAttributeInfo() {
    InitAttributeInfoMap();
}
