// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "DefaultGameplayCueManager.generated.h"

/**
 * 防止启动游戏每个gameplaycue触发，都会把所有gameplaycue以及粒子效果、声音文件都加载进内存
 */
UCLASS()
class AURA_API UDefaultGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()
	
protected:
    virtual bool ShouldAsyncLoadRuntimeObjectLibraries() const override;
};
