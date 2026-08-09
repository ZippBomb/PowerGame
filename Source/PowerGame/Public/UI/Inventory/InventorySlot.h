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
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UButton> button = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UItemIcon> icon = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget))
	TObjectPtr<UTextBlock> quantityText = nullptr;

	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UItemData> item;
	UPROPERTY(VisibleAnywhere)
	uint32 quantity = 0;

};