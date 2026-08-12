#pragma once

#include <CoreMinimal.h>
#include "UI/UserWidgetBase.h"

#include "InventoryPanel.generated.h"

class AMainPlayerController;

class UInventoryComponent;
struct FItemSlot;

class UInventorySlot;

class UWrapBox;

class UInputMappingContext;

UCLASS(Abstract)
class POWERGAME_API UInventoryPanel : public UUserWidgetBase {

	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UInventoryComponent> inventory = nullptr;

	UInventorySlot* AddSlot();
	void RemoveSlot(int32 index);

	void LoadSlots(const TArray<FItemSlot>& slots);

	inline UInventorySlot* GetSlot(uint32 index) const { return slots[index]; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UWrapBox> slotContainer = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlot> inventorySlotClass = nullptr;

private:
	UPROPERTY()
	TArray<TObjectPtr<UInventorySlot>> slots;

};