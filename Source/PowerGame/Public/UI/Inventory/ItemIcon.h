#pragma once

#include <CoreMinimal.h>
#include "UI/UserWidgetBase.h"

#include "ItemIcon.generated.h"

class UItemData;

class UInventorySlot;

class UImage;

UCLASS(Abstract)
class POWERGAME_API UItemIcon : public UUserWidgetBase {

	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetItem(UItemData* item);

	UFUNCTION(BlueprintCallable)
	inline void SetSlot(UInventorySlot* inventorySlot) { m_slot = inventorySlot; }
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UImage> icon = nullptr;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& geometry, const FPointerEvent& mouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& geometry, const FPointerEvent& mouseEvent, UDragDropOperation*& outOperation) override;

	UFUNCTION()
	void HandleDragCancelled(UDragDropOperation* operation);

private:
	UPROPERTY()
	TObjectPtr<UInventorySlot> m_slot = nullptr;

};