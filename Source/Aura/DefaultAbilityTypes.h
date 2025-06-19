#pragma once

#include "GameplayEffectTypes.h"

#include "DefaultAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FDefaultGameplayEffectContext : public FGameplayEffectContext {
    GENERATED_BODY()

public:
    virtual UScriptStruct *GetScriptStruct() const override;

    virtual bool NetSerialize(FArchive &Ar, class UPackageMap *Map,
                              bool &bOutSuccess) override;
    bool IsCriticalHit() const { return bIsCriticalHit; }

    bool IsBlockedHit() const { return bIsBlockedHit; }

    void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

    void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	/** Creates a copy of this context, used to duplicate for later modifications */

	virtual FDefaultGameplayEffectContext* Duplicate() const override
	{
		FDefaultGameplayEffectContext* NewContext = new FDefaultGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}


protected:
    UPROPERTY()
    bool bIsBlockedHit { false };

    UPROPERTY()
    bool bIsCriticalHit { false };
};

template<>
struct TStructOpsTypeTraits< FDefaultGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
