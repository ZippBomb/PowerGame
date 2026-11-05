#include "Building/Instances/Creative/CreativeOutput.h"

#include "Logistics/Conveyor.h"

ACreativeOutput::ACreativeOutput() {

	PrimaryActorTick.bCanEverTick = true;

}

void ACreativeOutput::Tick(float deltaTime) {

	Super::Tick(deltaTime);

	if (conveyor == nullptr || item == nullptr) return;

	if (!conveyor->CanReceiveItem(item)) return;
	conveyor->ReceiveItem(item);

}
