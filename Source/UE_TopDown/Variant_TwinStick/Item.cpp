// Fill out your copyright notice in the Description page of Project Settings.


#include "Variant_TwinStick/Item.h"
#include "Components/StaticMeshComponent.h"
#include "BasePlayerCharacter.h"

// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(SceneRoot);
	SetRootComponent(SceneRoot);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::Interact_Implementation(AActor* Interactor)
{
	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(Interactor);
	if (Player)
	{
		PickUp(Player);
	}
}

void AItem::PickUp_Implementation(ABasePlayerCharacter* ByCharacter)
{
	// Podstawowa implementacja - ukryj przedmiot
	SetActorHiddenInGame(true);
	//SetActorEnableCollision(false);
}

