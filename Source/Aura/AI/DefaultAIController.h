// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DefaultAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class AURA_API ADefaultAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    ADefaultAIController();

protected:

    UPROPERTY()
    TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
