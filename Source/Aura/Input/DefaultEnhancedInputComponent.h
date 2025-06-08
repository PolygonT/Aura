// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/DefaultInputConfig.h"
#include "DefaultEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDefaultEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
    template<class UserClass, typename HeldFuncType, typename PressedFuncType, typename ReleasedFuncType>
    void BindActionFromAbilities(
        const UDefaultInputConfig* InputConfig,
        UserClass* Object,
        HeldFuncType HeldFunc,
        PressedFuncType PressedFunc,
        ReleasedFuncType ReleasedFunc
    );

    void fun(const FGameplayTag& Tag) {}
};

template<class UserClass, typename HeldFuncType, typename PressedFuncType, typename ReleasedFuncType>
void UDefaultEnhancedInputComponent::BindActionFromAbilities(
        const UDefaultInputConfig* InputConfig,
        UserClass* Object,
        HeldFuncType HeldFunc,
        PressedFuncType PressedFunc,
        ReleasedFuncType ReleasedFunc
) {
    check(InputConfig);

    TObjectPtr<UInputAction> MoveAction;
    for (const auto& Pair : InputConfig->AbilityInputActionMap) {
        const FGameplayTag Tag {};
        //  TODO 2: template not working
        // BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::fun, Tag);
        // if (HeldFunc) {
        //     BindAction(Pair.Value, ETriggerEvent::Triggered, Object, HeldFunc, Pair.Key);
        // }

        // if (PressedFunc) {
        //     BindAction(Pair.Value, ETriggerEvent::Started, Object, PressedFunc, Pair.Key);
        // }
        //
        // if (ReleasedFunc) {
        //     BindAction(Pair.Value, ETriggerEvent::Completed, Object, ReleasedFunc, Pair.Key);
        // }
    }
    // InputConfig->AbilityInputActionMap
}
