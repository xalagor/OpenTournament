// Copyright (c) 2019-2020 Open Tournament Project, All Rights Reserved.

/////////////////////////////////////////////////////////////////////////////////////////////////

#include "UR_HealthBase.h"
#include "UR_Character.h"
#include "UR_AttributeSet.h"

/////////////////////////////////////////////////////////////////////////////////////////////////

AUR_HealthBase::AUR_HealthBase()
{
    HealAmount = 25;
    bSuperHeal = false;
}

bool AUR_HealthBase::AllowPickupBy_Implementation(class AActor* Other)
{
    if (Super::AllowPickupBy_Implementation(Other))
    {
        // Can always pick up super-heal items
        if (bSuperHeal)
        {
            return true;
        }

        // Can only pick ups health packs if we are below 100
        AUR_Character* Char = Cast<AUR_Character>(Other);
        return (Char && Char->AttributeSet && Char->AttributeSet->GetHealth() < Char->AttributeSet->GetHealthMax());
    }
    return false;
}

void AUR_HealthBase::GiveTo_Implementation(class AActor* Other)
{
    AUR_Character* Char = Cast<AUR_Character>(Other);
    if (Char && Char->AttributeSet)
    {
        // @! TODO : This is Temporary. Healing should be done via GameplayEffect.
        const int32 CurrentHealth = static_cast<int32>(Char->AttributeSet->GetHealth());
        const int32 FinalHealth = FMath::Min<int32>(CurrentHealth + HealAmount, Char->AttributeSet->GetHealthMax() + (bSuperHeal ? Char->AttributeSet->GetOverHealthMax() : 0));
        if (FinalHealth > CurrentHealth)
        {
            GAME_LOG(Game, Log, "Health Pickup: %d + %d -> %d", CurrentHealth, FinalHealth - CurrentHealth, FinalHealth);
            Char->AttributeSet->SetHealth(FinalHealth);
        }
    }

    Super::GiveTo_Implementation(Other);
}

FText AUR_HealthBase::GetItemName_Implementation()
{
    return FText::FromString(FString::Printf(TEXT("%i Health"), HealAmount));
}
