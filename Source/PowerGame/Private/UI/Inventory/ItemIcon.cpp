#include "UI/Inventory/ItemIcon.h"

#include <Components/Image.h>

void UItemIcon::SetIcon(UTexture2D* sprite) {

	icon->SetBrushFromTexture(sprite);

}