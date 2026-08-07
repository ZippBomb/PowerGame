#include "Building/Instances/BuildInstance.h"
#include "Building/Instances/Wire.h"

#include "Building/Build.h"
#include "Building/ConstructionModeManager.h"

#include "Player/MainPlayerCharacter.h"

#include "Power/PowerNetwork.h"

#include "Saving/WorldSaveSubsystem.h"
#include "Saving/BuildingSaveData.h"

#include "UI/Power/NetworkVisualizer.h"

#include <Kismet/GameplayStatics.h>

ABuildInstance::ABuildInstance() {

	PrimaryActorTick.bCanEverTick = false;

}

void ABuildInstance::BeginPlay() {

	Super::BeginPlay();

}

void ABuildInstance::SetBuild(UBuild* build, FGuid guid) {

	m_build = build;
	mesh->SetStaticMesh(build->mesh);

	if (guid.IsValid())
		m_guid = guid;
	else
		m_guid = FGuid::NewGuid();

}

void ABuildInstance::ConnectWire(AWire* wire) {

	m_connectedWires.Add(wire);

}

void ABuildInstance::Deconstruct() {

	for (TObjectPtr<AWire> wire : m_connectedWires) {

		if (wire == nullptr || wire->IsActorBeingDestroyed()) continue;
		if (m_powerNetwork != nullptr)
			m_powerNetwork->DisconnectWire(wire);

		GetWorld()->DestroyActor(wire);

	}

	if (m_powerNetwork)
		m_powerNetwork->DisconnectBuilding(this);

}

void ABuildInstance::SerializeSaveData(FInstancedStruct* out) {

	PW_ASSERT(out->GetScriptStruct()->IsChildOf(FBuildingSaveData::StaticStruct()), LogSaveSubsystem, TEXT("FInstancedStruct must be initialized by the most derived class."));

	FBuildingSaveData& saveData = out->GetMutable<FBuildingSaveData>();

	saveData.guid = m_guid;
	saveData.build = m_build;
	saveData.buildClass = GetClass();

	saveData.transform = GetActorTransform();

}
void ABuildInstance::DeserializeSaveData(const FInstancedStruct& data) {

	PW_ASSERT(data.GetScriptStruct()->IsChildOf(FBuildingSaveData::StaticStruct()), LogSaveSubsystem, TEXT("Saved FInstancedStruct is not of type FBuildingSaveData."));

	const FBuildingSaveData& saveData = data.Get<FBuildingSaveData>();
	SetBuild(saveData.build, saveData.guid);

}

void ABuildInstance::Interact(AMainPlayerCharacter* character) {

	PW_ASSERT(character != nullptr, LogBuilding, TEXT("ABuildInstance::Interact() received a nullptr character."));
	character->GetNetworkVisualizer()->Open(GetNetwork());

}

void ABuildInstance::SetHighlightMaterial(UMaterialInterface* material) {

	mesh->SetOverlayMaterial(material);

}
void ABuildInstance::ResetHighlightMaterial() {

	mesh->SetOverlayMaterial(nullptr);

}