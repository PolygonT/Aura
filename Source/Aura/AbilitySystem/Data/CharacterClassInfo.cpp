// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo&
UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass Key) {
    return CharacterClassMap[Key];
}

