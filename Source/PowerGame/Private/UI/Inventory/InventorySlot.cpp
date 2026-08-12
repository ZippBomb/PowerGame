#include "UI/Inventory/InventorySlot.h"
#include "UI/Inventory/ItemIcon.h"

#include "UI/MainHUD.h"

#include "Inventory/ItemSlot.h"
#include "Inventory/ItemData.h"

#include <Components/TextBlock.h>

#include <Blueprint/DragDropOperation.h>

#include <Engine/AssetManager.h>

void UInventorySlot::NativeConstruct() {

	Super::NativeConstruct();

	icon->SetSlot(this);

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

void UInventorySlot::Clear() {

	item = nullptr;

	icon->SetIcon(nullptr);
	UpdateQuantity(0);

}

bool UInventorySlot::NativeOnDrop(const FGeometry& geometry, const FDragDropEvent& event, UDragDropOperation* operation) {

	TObjectPtr<UItemDragPayload> payload = Cast<UItemDragPayload>(operation->Payload);
	if (payload == nullptr) return false;

	item = payload->item;
	
	icon->SetIcon(item->icon);
	UpdateQuantity(payload->quantity);

	return true;

}