#include "Building/Instances/Creative/CreativeOutput.h"

#include "Logistics/Conveyor.h"

ACreativeOutput::ACreativeOutput() {

	PrimaryActorTick.bCanEverTick = true;

}

void ACreativeOutput::Tick(float deltaTime) {

	Super::Tick(deltaTime);

	AConveyor* conveyor = slots[0].attachedConveyor;
	if (item == nullptr || conveyor == nullptr || !conveyor->CanReceiveItem(item)) return;

	conveyor->ReceiveItem(item);

}
