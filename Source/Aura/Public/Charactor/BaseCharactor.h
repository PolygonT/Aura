// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharactor.generated.h"

UCLASS(Abstract)
class AURA_API ABaseCharactor : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharactor();

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<USkeletalMeshComponent> Weapon;
};
