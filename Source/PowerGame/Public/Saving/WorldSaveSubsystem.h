#pragma once

#include <CoreMinimal.h>
#include <Subsystems/GameInstanceSubsystem.h>
#include <Logging/LogMacros.h>

#include "Core/Core.h"

#include "WorldSaveSubsystem.generated.h"

class UWorldSaveData;

class ABuildInstance;

DECLARE_LOG_CATEGORY_EXTERN(LogSaveSubsystem, Log, All)

UCLASS()
class POWERGAME_API UWorldSaveSubsystem : public UGameInstanceSubsystem {

	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

	UFUNCTION()
	void SaveWorld();
	UFUNCTION()
	void LoadWorld();

private:
	UFUNCTION()
	void SavingFinished(const FString& slotName, int32 userIndex, bool success);

	void SavePlayerData(UWorldSaveData* saveData);
	void SaveBuildings(UWorldSaveData* saveData);
	void SavePowerNetworks(UWorldSaveData* saveData);

	UFUNCTION()
	void LoadingFinished(const FString& slotName, int32 userIndex, USaveGame* loadedSave);

	void LoadPlayerData(const UWorldSaveData* saveData);
	void LoadBuildings(const UWorldSaveData* saveData, TMap<FGuid, TObjectPtr<ABuildInstance>>* buildingsMap);
	void LoadPowerNetworks(const UWorldSaveData* saveData, TMap<FGuid, TObjectPtr<ABuildInstance>>& buildingsMap);


};