#include "Saving/WorldSaveSubsystem.h"
#include "Saving/WorldSaveData.h"
#include "Saving/BuildingSaveData.h"
#include "Saving/NetworkSaveData.h"

#include "Player/MainPlayerCharacter.h"

#include "Building/Instances/BuildInstance.h"

#include "Power/PowerNetwork.h"

#include <Kismet/GameplayStatics.h>

#include <EngineUtils.h>
#include <StructUtils.h>

DEFINE_LOG_CATEGORY(LogSaveSubsystem)

void UWorldSaveSubsystem::Initialize(FSubsystemCollectionBase& collection) {

	Super::Initialize(collection);

}
void UWorldSaveSubsystem::Deinitialize() {

	Super::Deinitialize();

}

void UWorldSaveSubsystem::SaveWorld() {

	// Setup the save data object and delegate

	UWorldSaveData* saveData = Cast<UWorldSaveData>(UGameplayStatics::CreateSaveGameObject(UWorldSaveData::StaticClass()));
	PW_ASSERT(saveData != nullptr, LogSaveSubsystem, TEXT("Could not create UWorldSaveData save object."));

	FAsyncSaveGameToSlotDelegate saveDelegate;
	saveDelegate.BindUObject(this, &UWorldSaveSubsystem::SavingFinished);

	// Gather the data

	SavePlayerData(saveData);

	SaveBuildings(saveData);
	SavePowerNetworks(saveData);

	// Save the data to an actual save slot

	UGameplayStatics::AsyncSaveGameToSlot(saveData, "SaveSlot1", 0, saveDelegate);

}
void UWorldSaveSubsystem::SavingFinished(const FString& slotName, int32 userIndex, bool success) {

	PW_LOG(LogSaveSubsystem, TEXT("Saving finished into slot '%s'"), *slotName);

}

void UWorldSaveSubsystem::SavePlayerData(UWorldSaveData* saveData) {

	AMainPlayerCharacter* character = Cast<AMainPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PW_ASSERT(character != nullptr, LogSaveSubsystem, TEXT("Could not get AMainPlayerCharacter from UGameplayStatistics::GetPlayerCharacter()."));

	saveData->playerPosition = character->GetActorLocation();
	saveData->playerRotation = character->GetActorRotation();

}
void UWorldSaveSubsystem::SaveBuildings(UWorldSaveData* saveData) {

	for (TActorIterator<ABuildInstance> it(GetWorld()); it; ++it) {

		ABuildInstance* buildInstance = *it;
		FInstancedStruct entry;

		buildInstance->SerializeSaveData(&entry);
		saveData->buildings.Add(entry);

	}

}
void UWorldSaveSubsystem::SavePowerNetworks(UWorldSaveData* saveData) {

	for (TActorIterator<APowerNetwork> it(GetWorld()); it; ++it) {

		APowerNetwork* network = *it;
		FNetworkSaveData data;

		network->SerializeSaveData(&data);
		saveData->networks.Add(data);

	}

}

void UWorldSaveSubsystem::LoadWorld() {

	FAsyncLoadGameFromSlotDelegate loadDelegate;
	loadDelegate.BindUObject(this, &UWorldSaveSubsystem::LoadingFinished);

	UGameplayStatics::AsyncLoadGameFromSlot("SaveSlot1", 0, loadDelegate);

}
void UWorldSaveSubsystem::LoadingFinished(const FString& slotName, const int32 userIndex, USaveGame* loadedSaveData) {

	UWorldSaveData* saveData = Cast<UWorldSaveData>(loadedSaveData);
	PW_ASSERT(saveData != nullptr, LogSaveSubsystem, TEXT("Could not cast USaveGame to UWorldSaveData when loading save slot '%s'"), *slotName);

	// Load the actual data

	LoadPlayerData(saveData);

	TMap<FGuid, TObjectPtr<ABuildInstance>> buildingsMap;

	LoadBuildings(saveData, &buildingsMap);
	LoadPowerNetworks(saveData, buildingsMap);

}

void UWorldSaveSubsystem::LoadPlayerData(const UWorldSaveData* saveData) {

	AMainPlayerCharacter* character = Cast<AMainPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PW_ASSERT(character != nullptr, LogSaveSubsystem, TEXT("Could not get AMainPlayerCharacter from UGameplayStatistics::GetPlayerCharacter()."));

	character->SetActorLocationAndRotation(saveData->playerPosition, saveData->playerRotation);
	character->GetController()->SetControlRotation(saveData->playerRotation);

}
void UWorldSaveSubsystem::LoadBuildings(const UWorldSaveData* saveData, TMap<FGuid, TObjectPtr<ABuildInstance>>* buildingsMap) {

	UWorld* world = GetWorld();
	PW_ASSERT(world != nullptr, LogSaveSubsystem, TEXT("Could not get world."));

	for (const FInstancedStruct& data : saveData->buildings) {

		PW_ASSERT(data.GetScriptStruct()->IsChildOf(FBuildingSaveData::StaticStruct()), LogSaveSubsystem, TEXT("Saved FInstancedStructis not of type FBuildingSaveData."));
		const FBuildingSaveData& buildingSaveData = data.Get<FBuildingSaveData>();

		ABuildInstance* buildInstance = world->SpawnActor<ABuildInstance>(buildingSaveData.buildClass, buildingSaveData.transform);
		PW_ASSERT(buildInstance != nullptr, LogSaveSubsystem, TEXT("Could not spawn ABuildInstance actor of class '%s'"), *GetNameSafe(buildingSaveData.buildClass));

		buildInstance->DeserializeSaveData(data);
		buildingsMap->Add(buildingSaveData.guid, buildInstance);

	}

}
void UWorldSaveSubsystem::LoadPowerNetworks(const UWorldSaveData* saveData, TMap<FGuid, TObjectPtr<ABuildInstance>>& buildingsMap) {

	UWorld* world = GetWorld();
	PW_ASSERT(world != nullptr, LogSaveSubsystem, TEXT("Could not get world."));

	for (const FNetworkSaveData& data : saveData->networks) {

		APowerNetwork* network = world->SpawnActor<APowerNetwork>();
		PW_ASSERT(network != nullptr, LogSaveSubsystem, TEXT("Could not spawn APowerNetwork actor."));

		network->DeserializeSaveData(data, buildingsMap);

	}

}