// Fill out your copyright notice in the Description page of Project Settings.

#include "Variant_TwinStick/AttributesComponent.h"
#include "TimerManager.h"

UAttributesComponent::UAttributesComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    Health = MaxHealth;
    Mana = MaxMana;
    Score = 0; // int32
}

void UAttributesComponent::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;
    Mana = MaxMana;
}

void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (bIsRegeneratingMana && Mana < MaxMana)
    {
        SetMana(Mana + (ManaCost.ManaRegenRate * DeltaTime));
    }
}

void UAttributesComponent::SetHealth(float NewHealth)
{
    float OldHealth = Health;
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth); // ✅ Clamp do MaxHealth

    if (!FMath::IsNearlyEqual(Health, OldHealth))
    {
        OnHealthChanged.Broadcast(Health, MaxHealth);
    }

    if (Health <= 0.0f)
    {
        OnDeath.Broadcast();
    }
}


void UAttributesComponent::ApplyDamage(float Damage)
{
    if (Damage > 0.0f)
    {
        SetHealth(Health - Damage);
    }
}

void UAttributesComponent::SetMana(float NewMana)
{
    float OldMana = Mana;
    Mana = FMath::Clamp(NewMana, 0.0f, MaxMana); // ✅ Clamp do MaxMana

    if (!FMath::IsNearlyEqual(Mana, OldMana))
    {
        OnManaChanged.Broadcast(Mana, MaxMana);
    }
}

void UAttributesComponent::PayMana(float Cost)
{
    if (Cost <= 0.0f || Mana < Cost) return;

    SetMana(Mana - Cost);
    StopManaRegeneration();

    FTimerHandle RegenerationTimer;
    GetWorld()->GetTimerManager().SetTimer(
        RegenerationTimer,
        this,
        &UAttributesComponent::StartManaRegeneration,
        1.0f,
        false
    );
}

void UAttributesComponent::AddScore(int32 Points)
{
    Score = FMath::Max(0, Score + Points);
    OnScoreChanged.Broadcast(Score); // ← int32!
}