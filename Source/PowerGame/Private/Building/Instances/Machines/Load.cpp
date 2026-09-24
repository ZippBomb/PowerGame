#include "Building/Instances/Machines/Load.h"

#include "Power/PowerNetwork.h"

#include "Saving/WorldSaveSubsystem.h"

void ALoad::EndPlay(const EEndPlayReason::Type reason) {

	Super::EndPlay(reason);

	if (m_powerNetwork == nullptr) return;

	//m_powerNetwork->DisconnectLoad(this);
	m_powerNetwork = nullptr;

}

void ALoad::SerializeSaveData(FInstancedStruct* out) {

	PW_ASSERT(!out->IsValid(), LogSaveSubsystem, TEXT("FInstancedStruct should be initialized by the most derived class. '%s'"), *GetNameSafe(this));
	out->InitializeAs<FLoadSaveData>();

	Super::SerializeSaveData(out);

	FLoadSaveData& saveData = out->GetMutable<FLoadSaveData>();

	saveData.demand = demand;

}
void ALoad::DeserializeSaveData(const FInstancedStruct& data) {

	PW_ASSERT(data.GetScriptStruct()->IsChildOf(FLoadSaveData::StaticStruct()), LogSaveSubsystem, TEXT("Saved FInstancedStruct is not of type FLoadSaveData."));
	Super::DeserializeSaveData(data);

	const FLoadSaveData& saveData = data.Get<FLoadSaveData>();

	demand = saveData.demand;

}