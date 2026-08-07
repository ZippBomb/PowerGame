#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

#include "InventoryComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class POWERGAME_API UInventoryComponent : public UActorComponent {

	GENERATED_BODY()

public:
	UInventoryComponent();

	void AddItem(const FPrimaryAssetId& itemID, uint32 quantity = 1);
	bool RemoveItem(const FPrimaryAssetId& itemID, uint32 quantity = 1);

private:
	UPROPERTY(VisibleAnywhere)
	TMap<FPrimaryAssetId, uint32> storedItems;

	virtual void BeginPlay() override;
	
};