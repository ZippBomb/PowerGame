#include "Logistics/Conveyor.h"
#include "Logistics/ItemReceiver.h"

#include "Inventory/ItemData.h"

#include <Components/InstancedStaticMeshComponent.h>
#include <Components/SplineComponent.h>
#include <Components/SplineMeshComponent.h>

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
		item.distance += (speed * deltaTime);

		if (item.distance >= m_length) {

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

		transform.SetLocation(m_spline->GetLocationAtDistanceAlongSpline(item.distance, ESplineCoordinateSpace::Local));
		item.ism->UpdateInstanceTransformById(item.ismID, transform, false, true);

	}

	// RemoveAt checks if Count is 0 and does nothing, no need to check it twice
	items.RemoveAt(0, arrivedCounter);

}

void AConveyor::InsertItem(UItemData* item) {

	FConveyorItem& newItem = items.AddDefaulted_GetRef();

	newItem.item = item;
	newItem.distance = 0.0f;

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
		newItem.ism->SetRelativeLocation(FVector(0.0f, 0.0f, conveyorHeight));

		newItem.ism->SetGenerateOverlapEvents(false);
		newItem.ism->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem.ism->SetCollisionResponseToAllChannels(ECR_Ignore);

		newItem.ism->SetStaticMesh(item->conveyorMesh);
		itemISMMap.Add(item, newItem.ism);

	}

	FTransform transform;
	transform.SetLocation(m_spline->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::Local));

	newItem.ismID = newItem.ism->AddInstanceById(transform);

	insertTimer = 1 / insertSpeed;

}

void AConveyor::SetSpline(USplineComponent* spline) {

	FVector startPos, startTangent;
	FVector endPos, endTangent;

	spline->GetLocationAndTangentAtSplinePoint(0, startPos, startTangent, ESplineCoordinateSpace::Local);
	spline->GetLocationAndTangentAtSplinePoint(1, endPos, endTangent, ESplineCoordinateSpace::Local);

	m_spline->SetLocationAtSplinePoint(0, startPos, ESplineCoordinateSpace::Local);
	m_spline->SetTangentAtSplinePoint(0, startTangent, ESplineCoordinateSpace::Local);

	m_spline->SetLocationAtSplinePoint(1, endPos, ESplineCoordinateSpace::Local);
	m_spline->SetTangentAtSplinePoint(1, endTangent, ESplineCoordinateSpace::Local);

	m_splineMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent);
	m_length = m_spline->GetSplineLength();

	slots[1].transform.SetLocation(slots[0].transform.GetLocation() + endPos);

}
