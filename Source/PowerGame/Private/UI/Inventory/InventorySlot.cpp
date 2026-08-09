#include "UI/Inventory/InventorySlot.h"
#include "UI/Inventory/ItemIcon.h"

#include "UI/MainHUD.h"

#include "Inventory/ItemSlot.h"
#include "Inventory/ItemData.h"

#include <Components/TextBlock.h>

#include <Engine/AssetManager.h>

void UInventorySlot::NativeConstruct() {

	//

}

void UInventorySlot::SetData(const FItemSlot& data) {

	PW_ASSERT(data.item != nullptr, LogUI, TEXT("A UInventorySlot can't have an invalid item."));

	item = data.item;
	icon->SetIcon(item->icon);

	UpdateQuantity(data.quantity);


}

void UInventorySlot::UpdateQuantity(uint32 value) {

	quantity = value;
	quantityText->SetText(FText::FromString(FString::FromInt(value)));

}