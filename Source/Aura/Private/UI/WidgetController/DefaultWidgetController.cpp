// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/DefaultWidgetController.h"


void UDefaultWidgetController::SetWidgetControllerMemberVariables(const FWidgetControllerParams& Param) {
    PlayerController = Param.PlayerController;
    PlayerState = Param.PlayerState;
    AbilitySystemComponent = Param.AbilitySystemComponent;
    AttributeSet = Param.AttributeSet;
}

void UDefaultWidgetController::BroadcastInitialValues() {

}

void UDefaultWidgetController::BindCallbacksToDependencies() {

}
