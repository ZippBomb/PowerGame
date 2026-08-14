#include "Building/Instances/Machines/MachineBuildInstance.h"

#include "Building/Builds/Machine.h"

#include "Player/MainPlayerCharacter.h"

#include "UI/Buildings/Machines/MachineUI.h"

#include "UI/MainLayout.h"

DEFINE_LOG_CATEGORY(LogMachines);

AMachineBuildInstance::AMachineBuildInstance() {

	PrimaryActorTick.bCanEverTick = false;

}

void AMachineBuildInstance::Interact(AMainPlayerCharacter* character) {

	UMachine* machine = Cast<UMachine>(GetBuild());
	PW_ASSERT(machine != nullptr, LogMachines, TEXT("AMachineBuildInstance must have a Build of UMachine type."));

	if (ui == nullptr)
		ui = character->GetUI()->GetMachineUI(machine->uiClass);

	ui->Open(this);
	uiOpen = true;

}