#pragma once

#include <CoreMinimal.h>
#include "UI/UserWidgetBase.h"

#include "InventoryPanel.generated.h"

class AMainPlayerController;

class UInventorySlot;
struct FItemSlot;

class UWrapBox;

class UInputMappingContext;

UCLASS(Abstract)
class POWERGAME_API UInventoryPanel : public UUserWidgetBase {

	GENERATED_BODY()

public:
	UInventorySlot* AddSlot(const FItemSlot& data);
	void RemoveSlot(int32 index);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UWrapBox> slotContainer = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlot> inventorySlotClass = nullptr;

private:
	UPROPERTY()
	TArray<TObjectPtr<UInventorySlot>> slots;

};