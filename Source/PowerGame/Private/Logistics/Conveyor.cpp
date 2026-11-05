#include "Logistics/Conveyor.h"
#include "Logistics/ItemReceiver.h"

#include "Inventory/ItemData.h"

#include <Components/InstancedStaticMeshComponent.h>

DEFINE_LOG_CATEGORY(LogLogistics);

AConveyor::AConveyor() {

	PrimaryActorTick.bCanEverTick = true;

}

void AConveyor::Tick(float deltaTime) {

	Super::Tick(deltaTime);

	if (insertTimer >= 0.0f)
		insertTimer -= deltaTime;

	FTransform transform;
	uint32 arrivedCounter = 0;

	for (int32 i = 0; i < items.Num(); i++) {

		FConveyorItem& item = items[i];
		item.progress += (speed * deltaTime) / length;

		if (item.progress >= 1.0f) {

			arrivedCounter++;
			if (connectedReceiver != nullptr && connectedReceiver->CanReceiveItem(item.item))
				connectedReceiver->ReceiveItem(item.item);

			item.ism->RemoveInstanceById(item.ismID);
			if (item.ism->GetNumInstances() == 0) {

				item.ism->SetVisibility(false);

				itemISMMap.Remove(item.item);
				ismPool.Push(item.ism);

			}

			continue;

		}

		// NOTE: UpdateInstanceTransformById seems to have a mistake where it only takes in 2 arguments and passes bTeleport as bMarkRenderStateDirty
		//		 when calling UpdateInstanceTransform

		transform.SetLocation(start + (end - start) * item.progress);
		item.ism->UpdateInstanceTransformById(item.ismID, transform, false, true);

	}

	// RemoveAt checks if Count is 0 and does nothing, no need to check it twice
	items.RemoveAt(0, arrivedCounter);

}

void AConveyor::InsertItem(UItemData* item) {

	FConveyorItem& newItem = items.AddDefaulted_GetRef();

	newItem.item = item;
	newItem.progress = 0.0f;

	if (itemISMMap.Contains(item))
		newItem.ism = itemISMMap[item];
	else if (!ismPool.IsEmpty()) {

		newItem.ism = ismPool.Pop();
		PW_ASSERT(newItem.ism != nullptr, LogLogistics, TEXT("ISM Component from ISM pool is invalid."));

		newItem.ism->SetVisibility(true);
		newItem.ism->SetStaticMesh(item->conveyorMesh);

		itemISMMap.Add(item, newItem.ism);

	} else {

		newItem.ism = NewObject<UInstancedStaticMeshComponent>(this);

		newItem.ism->bAffectDistanceFieldLighting = false;
		newItem.ism->bCastContactShadow = false;
		newItem.ism->bVisibleInRayTracing = false;
		newItem.ism->bAffectDynamicIndirectLighting = false;

		newItem.ism->SetupAttachment(RootComponent);
		newItem.ism->RegisterComponent();

		newItem.ism->SetGenerateOverlapEvents(false);
		newItem.ism->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem.ism->SetCollisionResponseToAllChannels(ECR_Ignore);

		newItem.ism->SetStaticMesh(item->conveyorMesh);
		itemISMMap.Add(item, newItem.ism);

	}

	FTransform transform;
	transform.SetLocation(start);

	newItem.ismID = newItem.ism->AddInstanceById(transform);

	insertTimer = 1 / insertSpeed;

}
