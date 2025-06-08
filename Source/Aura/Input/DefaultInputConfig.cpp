// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/DefaultInputConfig.h"
#include "Logging/LogMacros.h"

const UInputAction *
UDefaultInputConfig::FindAbilityInputActionByTag(const FGameplayTag &InputTag,
                                                 bool bLogNotFound) const {
    if (!AbilityInputActionMap.Contains(InputTag)) {
        if (bLogNotFound) {
            UE_LOG(LogTemp, Error, TEXT("Can't find Info for InputTag [%s], on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
        }

        return nullptr;
    }

    return AbilityInputActionMap[InputTag];
}

