#include "Building/Instances/Power/Wire.h"

#include "Power/PowerNetwork.h"

#include "Saving/WorldSaveSubsystem.h"

void AWire::Connect(ABuildInstance* startBuildInstance, ABuildInstance* endBuildInstance) {

	m_startBuildInstance = startBuildInstance;
	m_startGUID = startBuildInstance->GetGUID();

	m_endBuildInstance = endBuildInstance;
	m_endGUID = endBuildInstance->GetGUID();

	m_powerNetwork = APowerNetwork::HandleConnection(startBuildInstance, endBuildInstance, this);

	startBuildInstance->ConnectWire(this);
	endBuildInstance->ConnectWire(this);

}

void AWire::SerializeSaveData(FInstancedStruct* out) {

	PW_ASSERT(!out->IsValid(), LogSaveSubsystem, TEXT("FInstancedStruct should be initialized by the most derived class. '%s'"), *GetNameSafe(this));
	out->InitializeAs<FWireSaveData>();

	Super::SerializeSaveData(out);

	FWireSaveData& saveData = out->GetMutable<FWireSaveData>();

	saveData.startGUID = m_startGUID;
	saveData.endGUID = m_endGUID;

}

void AWire::DeserializeSaveData(const FInstancedStruct& data) {

	PW_ASSERT(data.GetScriptStruct()->IsChildOf(FWireSaveData::StaticStruct()), LogSaveSubsystem, TEXT("Saved FInstancedStruct is not of type FWireSaveData."));
	Super::DeserializeSaveData(data);

	const FWireSaveData& saveData = data.Get<FWireSaveData>();

	m_startGUID = saveData.startGUID;
	m_endGUID = saveData.endGUID;

}