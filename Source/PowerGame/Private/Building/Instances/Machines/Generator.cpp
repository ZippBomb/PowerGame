#include "Building/Instances/Machines/Generator.h"

#include "Power/PowerNetwork.h"

#include "Saving/WorldSaveSubsystem.h"

#include "PowerGameMode.h"

void AGenerator::BeginPlay() {

	Super::BeginPlay();

	//

}
void AGenerator::EndPlay(const EEndPlayReason::Type reason) {

	Super::EndPlay(reason);

	if (m_powerNetwork == nullptr) return;

	//m_powerNetwork->DisconnectGenerator(this);
	m_powerNetwork = nullptr;

}

void AGenerator::SerializeSaveData(FInstancedStruct* out) {

	PW_ASSERT(!out->IsValid(), LogSaveSubsystem, TEXT("FInstancedStruct should be initialized by the most derived class. '%s'"), *GetNameSafe(this));
	out->InitializeAs<FGeneratorSaveData>();

	Super::SerializeSaveData(out);

	FGeneratorSaveData& saveData = out->GetMutable<FGeneratorSaveData>();

	saveData.maxOutput = maxOutput;

}
void AGenerator::DeserializeSaveData(const FInstancedStruct& data) {

	PW_ASSERT(data.GetScriptStruct()->IsChildOf(FGeneratorSaveData::StaticStruct()), LogSaveSubsystem, TEXT("Saved FInstancedStruct is not of type FGeneratorSaveData."));
	Super::DeserializeSaveData(data);
	
	const FGeneratorSaveData& saveData = data.Get<FGeneratorSaveData>();

	maxOutput = saveData.maxOutput;

}