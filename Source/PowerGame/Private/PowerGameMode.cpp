#include "PowerGameMode.h"

#include "Saving/WorldSaveSubsystem.h"

DEFINE_LOG_CATEGORY(LogPowerGame)

void APowerGameMode::BeginPlay() {

	Super::BeginPlay();

	UPowerGameInstance* gameInstance = Cast<UPowerGameInstance>(GetGameInstance());
	PW_ASSERT(gameInstance != nullptr, LogPowerGame, TEXT("Could not get UPowerGameInstance."));

	if (!gameInstance->pendingSaveLoad) return;

	UWorldSaveSubsystem* saveSubsystem = gameInstance->GetSubsystem<UWorldSaveSubsystem>();
	PW_ASSERT(saveSubsystem != nullptr, LogPowerGame, TEXT("Could not get UWorldSaveSubsystem from UPowerGameInstance."));

	saveSubsystem->LoadWorld(gameInstance->pendingSaveSlot);

}