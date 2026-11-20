#include "Building/BuildGhostConveyor.h"
#include "Building/ConstructionModeManager.h"

#include "Building/Instances/BuildInstance.h"

#include "Logistics/ItemReceiver.h"
#include "Logistics/Conveyor.h"

void ABuildGhostConveyor::Update(const FVector& hitLocation, ABuildInstance* hitBuildInstance) {

	if (hitBuildInstance == nullptr || !hitBuildInstance->Implements<UItemReceiver>()) {

		Super::Update(hitLocation, hitBuildInstance);
		return;

	}

	IItemReceiver* receiver = Cast<IItemReceiver>(hitBuildInstance);
	PW_ASSERT(receiver != nullptr, LogLogistics, TEXT("Build instance that implements IItemReceiver failed to cast."));

	TArray<FMachineItemSlot>& slots = receiver->GetMachineSlots();

	FMachineItemSlot* bestSlot = nullptr;
	float bestDistance = MAX_FLT;
	for (FMachineItemSlot& slot : slots) {

		if (slot.attachedConveyor != nullptr) continue;

		const FVector worldLocation = hitBuildInstance->GetActorTransform().TransformPosition(slot.transform.GetLocation());
		const float distance = FVector::DistSquared(hitLocation, worldLocation);

		if (distance >= bestDistance) continue;

		bestDistance = distance;
		bestSlot = &slot;

	}
	if (bestDistance > maxSnapDistance * maxSnapDistance)
		bestSlot = nullptr;

	machineSlot = bestSlot;

	if (machineSlot == nullptr) {

		Super::Update(hitLocation, hitBuildInstance);
		return;

	}

	FTransform connectionPointTransform = hitBuildInstance->GetActorTransform() * machineSlot->transform;

	SetActorLocation(connectionPointTransform.GetLocation());
	SetActorRotation(connectionPointTransform.GetRotation());

	SetActorHiddenInGame(false);

}

ABuildInstance* ABuildGhostConveyor::Confirm(const FVector& location, ABuildInstance* targetBuildInstance) {

	if (machineSlot == nullptr)
		return Super::Confirm(GetActorLocation(), targetBuildInstance);

	PW_ASSERT(targetBuildInstance != nullptr, LogBuilding, TEXT("Target build instance is nullptr."));

	AConveyor* instance = Cast<AConveyor>(Super::Confirm(GetActorLocation(), targetBuildInstance));
	PW_ASSERT(instance != nullptr, LogBuilding, TEXT("Could not cast spawned actor of type ABuildInstance to AConveyor."));

	machineSlot->attachedConveyor = instance;

	if (machineSlot->input) {

		PW_ASSERT(targetBuildInstance->Implements<UItemReceiver>(), LogBuilding, TEXT("Target build instance does not implement IItemReceiver....... how"));
		TScriptInterface<IItemReceiver> receiver = targetBuildInstance;
		PW_ASSERT(receiver != nullptr, LogLogistics, TEXT("Build instance that implements IItemReceiver failed to cast."));

		instance->connectedReceiver = receiver;

	} else if (AConveyor* target = Cast<AConveyor>(targetBuildInstance)) {

		target->connectedReceiver = instance;

	}

	return instance;


}
