#include "UI/Inventory/ItemIcon.h"
#include "UI/Inventory/InventorySlot.h"

#include "Inventory/ItemData.h"

#include <Components/Image.h>

#include <Blueprint/WidgetBlueprintLibrary.h>

void UItemIcon::SetIcon(UTexture2D* sprite) {

	icon->SetBrushFromTexture(sprite);

	if (sprite == nullptr)
		icon->SetOpacity(0.0f);
	else
		icon->SetOpacity(1.0f);

}

FReply UItemIcon::NativeOnMouseButtonDown(const FGeometry& geometry, const FPointerEvent& mouseEvent) {

	if (!mouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) return Super::NativeOnMouseButtonDown(geometry, mouseEvent);
	return UWidgetBlueprintLibrary::DetectDragIfPressed(mouseEvent, this, EKeys::LeftMouseButton).NativeReply;

}

void UItemIcon::NativeOnDragDetected(const FGeometry& geometry, const FPointerEvent& mouseEvent, UDragDropOperation*& outOperation) {

	TObjectPtr<UDragDropOperation> operation = NewObject<UDragDropOperation>();
	TObjectPtr<UItemIcon> visual = CreateWidget<UItemIcon>(GetOwningPlayer(), GetClass());

	if (visual == nullptr) return;
	visual->SetIcon(m_slot->GetItem()->icon);

	TObjectPtr<UItemDragPayload> payload = NewObject<UItemDragPayload>();
	payload->item = m_slot->GetItem();
	payload->quantity = m_slot->GetQuantity();
	payload->slot = m_slot;

	operation->Payload = Cast<UObject>(payload);
	operation->DefaultDragVisual = visual;
	operation->OnDragCancelled.AddDynamic(this, &UItemIcon::HandleDragCancelled);
	outOperation = operation;

	m_slot->Clear();

	// Remove item from inventory ?

}

void UItemIcon::HandleDragCancelled(UDragDropOperation* operation) {

	//

}