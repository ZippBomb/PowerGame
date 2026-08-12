#pragma once

#include <CoreMinimal.h>
#include "UI/UserWidgetBase.h"
#include "Core/Core.h"

#include "InventorySlot.generated.h"

struct FItemSlot;
class UItemData;

class UItemIcon;

class UButton;
class UTextBlock;

UCLASS(Abstract)
class POWERGAME_API UInventorySlot : public UUserWidgetBase {

	GENERATED_BODY()

public:
	void SetData(const FItemSlot& data);

	void UpdateQuantity(uint32 value);

	void Clear();

	inline UItemData* GetItem() const { return item; }
	inline uint32 GetQuantity() const { return quantity; }
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UButton> button = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UItemIcon> icon = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> quantityText = nullptr;

	virtual void NativeConstruct() override;

	virtual bool NativeOnDrop(const FGeometry& geometry, const FDragDropEvent& event, UDragDropOperation* operation) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UItemData> item;
	UPROPERTY(VisibleAnywhere)
	uint32 quantity = 0;

};

UCLASS()
class POWERGAME_API UItemDragPayload : public UObject {

	GENERATED_BODY()

public:
	TObjectPtr<UItemData> item = nullptr;
	uint32 quantity = 0;

	TObjectPtr<UInventorySlot> slot = nullptr;

};