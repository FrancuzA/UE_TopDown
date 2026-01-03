// Fill out your copyright notice in the Description page of Project Settings.


#include "Variant_TwinStick/AttributesComponent.h"

// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    Health = MaxHealth;
    Mana = MaxMana;
    Score = 0.0f;
}


// Called when the game starts
void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	Mana = MaxMana;
}


// Called every frame
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
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

    if (Health != OldHealth)
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
    SetHealth(Health - Damage);
}

void UAttributesComponent::SetMana(float NewMana)
{
    float ClampedMana = FMath::Clamp(NewMana, 0.0f, MaxMana);
    if (FMath::IsNearlyEqual(Mana, ClampedMana)) return;

    Mana = ClampedMana;
    OnManaChanged.Broadcast(Mana, MaxMana);
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

void UAttributesComponent::AddScore(float Points)
{
    Score = FMath::Max(0.0f, Score + Points);
    OnScoreChanged.Broadcast(Score);
}

