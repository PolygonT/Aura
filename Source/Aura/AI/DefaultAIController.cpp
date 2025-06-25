// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DefaultAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ADefaultAIController::ADefaultAIController() {
    Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
    // BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
}

