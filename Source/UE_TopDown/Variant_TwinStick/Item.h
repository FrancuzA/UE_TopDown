#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Health,
    Mana,
    Weapon
};

UCLASS()
class UE_TOPDOWN_API AItem : public AActor, public IInteractionInterface
{
    GENERATED_BODY()

public:
    AItem();

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    EItemType ItemType = EItemType::Health;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    float HealthAmount = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    float ManaAmount = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TSubclassOf<class AProjectile> WeaponClass;

    virtual void Interact_Implementation(AActor* Interactor) override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Item")
    void PickUp(ABasePlayerCharacter* ByCharacter);
    virtual void PickUp_Implementation(ABasePlayerCharacter* ByCharacter);
};