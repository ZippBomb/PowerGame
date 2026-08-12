#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

#include "Inventory/ItemSlot.h"

#include "InventoryComponent.generated.h"

class UItemData;

class UInventoryPanel;

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class POWERGAME_API UInventoryComponent : public UActorComponent {

	GENERATED_BODY()

public:
	UInventoryComponent();

	void AddItem(UItemData* item, uint32 quantity = 1);
	bool RemoveItem(UItemData* item, uint32 quantity = 1);
	
	void SetAtSlot(uint32 index, UItemData* item, uint32 quantity = 1);
	bool RemoveFromSlot(uint32 index, uint32 quantity = 0);

	UFUNCTION(BlueprintCallable)
	void DisplayInventory(UInventoryPanel* inventoryPanel);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Items")
	TMap<TObjectPtr<UItemData>, uint32> storedItems;

	UPROPERTY(EditAnywhere, Category = "Slots")
	uint32 numOfSlots = 18;
	UPROPERTY(VisibleAnywhere, Category = "Slots")
	TArray<FItemSlot> slots;

	UPROPERTY()
	TObjectPtr<UInventoryPanel> inventoryPanel = nullptr;

	virtual void BeginPlay() override;
	
};