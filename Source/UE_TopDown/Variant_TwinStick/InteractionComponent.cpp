// Fill out your copyright notice in the Description page of Project Settings.


#include "Variant_TwinStick/InteractionComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractionComponent::SphereTrace(FHitResult& SphereHit)
{
	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(GetOwner());
	for (AActor* Actor : IgnoreActors)
	{
		ActorToIgnore.AddUnique(Actor);
	}
	ABasePlayerCharacter* CppOwner = Cast<ABasePlayerCharacter>(GetOwner());

	FVector EndTrace = CppOwner->GetCameraLocation() + (CppOwner->GetCameraForwardVector() * 500.f);

	UKismetSystemLibrary::SphereTraceSingle(

		this,
		CppOwner->GetCameraLocation(),
		EndTrace,
		TraceSphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorToIgnore,
		EDrawDebugTrace::ForDuration,
		SphereHit,
		true
	);
}

void UInteractionComponent::TryInteract()
{
	FHitResult SphereHit;
	SphereTrace(SphereHit);

	AActor* HitActor = SphereHit.GetActor();
	if (!HitActor) return;

	if (HitActor->Implements<UInteractionInterface>())
	{
		IInteractionInterface::Execute_Interact(HitActor, GetOwner());
		IgnoreActors.Add(HitActor);
	}
}

