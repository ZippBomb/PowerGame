#pragma once

#include <CoreMinimal.h>
#include "UI/UserWidgetBase.h"
#include "Core/Core.h"

#include "InventorySlot.generated.h"

struct FItemSlot;
class UItemData;

class UInventoryPanel;
class UItemIcon;

class UButton;
class UTextBlock;

UCLASS(Abstract)
class POWERGAME_API UInventorySlot : public UUserWidgetBase {

	GENERATED_BODY()

	friend class UInventoryPanel;

public:
	void SetData(const FItemSlot& data);

	void SetItem(UItemData* item);
	void SetQuantity(uint32 value);

	void Clear();

	inline UItemData* GetItem() const { return item; }
	inline uint32 GetQuantity() const { return quantity; }

	inline uint32 GetSlotIndex() const { return slotIndex; }
	inline UInventoryPanel* GetInventoryPanel() const { return inventoryPanel; }
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UButton> button = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UItemIcon> icon = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> quantityText = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UItemData> item;
	UPROPERTY(VisibleAnywhere)
	uint32 quantity = 0;

	UPROPERTY()
	uint32 slotIndex = 0;
	UPROPERTY()
	TObjectPtr<UInventoryPanel> inventoryPanel = nullptr;

	virtual void NativeConstruct() override;

	virtual bool NativeOnDrop(const FGeometry& geometry, const FDragDropEvent& event, UDragDropOperation* operation) override;

	virtual bool CanAcceptDrop(UItemData* droppedItem) const { return true; }

};

UCLASS()
class POWERGAME_API UItemDragPayload : public UObject {

	GENERATED_BODY()

public:
	TObjectPtr<UItemData> item = nullptr;
	uint32 quantity = 0;

	TObjectPtr<UInventorySlot> slot = nullptr;

};