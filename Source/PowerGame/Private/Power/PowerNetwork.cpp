#include "Power/PowerNetwork.h"

#include "Building/Instances/Power/Generator.h"
#include "Building/Instances/Power/Load.h"
#include "Building/Instances/Power/Wire.h"

#include "Saving/NetworkSaveData.h"
#include "Saving/WorldSaveSubsystem.h"

DEFINE_LOG_CATEGORY(LogPower);

APowerNetwork::APowerNetwork() {

	PrimaryActorTick.bCanEverTick = true;

}

void APowerNetwork::Tick(float deltaTime) {

	Super::Tick(deltaTime);

	// First calculate the total supply and demand

	m_totalSupply = 0.0f;
	m_totalDemand = 0.0f;

	for (TObjectPtr<AGenerator> generator : m_generators)
		m_totalSupply += generator->GetMaxOutput();

	for (TObjectPtr<ALoad> load : m_loads)
		m_totalDemand += load->GetDemand();

	if (m_totalSupply == 0.0f) {

		m_dead = true;
		m_frequency = 0.0f;
		m_voltage = 0.0f;

		return;

	}

	if (m_totalDemand == 0.0f) return;

	// Calculate grid frequency

	float imbalance = m_totalSupply - m_totalDemand;
	m_frequency += imbalance * responseStrength * deltaTime;

	// Update generators

	float error = baseFrequency - m_frequency;
	float totalGeneration = 0.0f;

	for (TObjectPtr<AGenerator> generator : m_generators) {

		generator->Respond(error);
		totalGeneration += generator->GetCurrentOutput();

	}

	// Update loads

	float supplyRatio = FMath::Clamp(totalGeneration / m_totalDemand, 0.0f, 1.0f);

	for (TObjectPtr<ALoad> load : m_loads)
		load->Update(supplyRatio);

	// Update voltage
	// TODO: This is just basic and crude, this should be updated

	m_voltage = baseVoltage * FMath::Clamp(supplyRatio, 0.9f, 1.1f);

}

void APowerNetwork::ConnectGenerator(AGenerator* generator) {

	PW_ASSERT(generator != nullptr, LogPower, TEXT("Can't connect invalid generator to power network '%s'."), *GetNameSafe(this));
	m_generators.Add(generator);

}
void APowerNetwork::ConnectLoad(ALoad* load) {

	PW_ASSERT(load != nullptr, LogPower, TEXT("Can't connect invalid load to power network '%s'."), *GetNameSafe(this));
	m_loads.Add(load);

}

void APowerNetwork::DisconnectGenerator(AGenerator* generator) {

	PW_ASSERT(m_generators.Contains(generator), LogPower, TEXT("Can't disconnect generator ('%s') that isn't connected to network: '%s'"), *GetNameSafe(generator), *GetNameSafe(this));
	m_generators.Remove(generator);

}
void APowerNetwork::DisconnectLoad(ALoad* load) {

	PW_ASSERT(m_loads.Contains(load), LogPower, TEXT("Can't disconnect load ('%s') that isn't connected to network: '%s'"), *GetNameSafe(load), *GetNameSafe(this));
	m_loads.Remove(load);
	
}
void APowerNetwork::DisconnectWire(AWire* wire) {

	PW_ASSERT(m_connections.Contains(wire), LogPower, TEXT("Can't disconnect wire ('%s') that isn't connected to network: '%s'"), *GetNameSafe(wire), *GetNameSafe(this));
	m_connections.Remove(wire);

}
void APowerNetwork::DisconnectBuilding(ABuildInstance* building) {

	if (AGenerator* generator = Cast<AGenerator>(building))
		DisconnectGenerator(generator);
	else if (ALoad* load = Cast<ALoad>(building))
		DisconnectLoad(load);
	else if (AWire* wire = Cast<AWire>(building))
		DisconnectWire(wire);
	else {

		PW_ASSERT(m_miscBuildInstances.Contains(building), LogPower, TEXT("Can't disconnect misc building ('%s') that isn't connect to network: '%s'"), *GetNameSafe(building), *GetNameSafe(this));
		m_miscBuildInstances.Remove(building);

	}

}

void APowerNetwork::SerializeSaveData(FNetworkSaveData* out) {

	out->baseFrequency = baseFrequency;
	out->baseVoltage = baseVoltage;
	out->responseStrength = responseStrength;

	out->dead = m_dead;
	out->frequency = m_frequency;
	out->voltage = m_voltage;

	for (TObjectPtr<AGenerator> generator : m_generators) {

		PW_ASSERT(generator != nullptr, LogPower, TEXT("Can't serialize an invalid generator on Network '%s'"), *GetNameSafe(this));
		out->generators.Add(generator->GetGUID());

	}
	for (TObjectPtr<ALoad> load : m_loads) {

		PW_ASSERT(load != nullptr, LogPower, TEXT("Can't serialize an invalid load on Network '%s'"), *GetNameSafe(this));
		out->loads.Add(load->GetGUID());

	}
	for (TObjectPtr<ABuildInstance> misc : m_miscBuildInstances) {

		PW_ASSERT(misc != nullptr, LogPower, TEXT("Can't serialize an invalid load on Network '%s'"), *GetNameSafe(this));
		out->misc.Add(misc->GetGUID());

	}

	for (TObjectPtr<AWire> wire : m_connections) {

		PW_ASSERT(wire != nullptr, LogPower, TEXT("Can't serialize an invalid wire on Network '%s'"), *GetNameSafe(this));
		out->connections.Add(wire->GetGUID());

	}

}
void APowerNetwork::DeserializeSaveData(const FNetworkSaveData& data, TMap<FGuid, TObjectPtr<ABuildInstance>>& buildingsMap) {

	baseFrequency = data.baseFrequency;
	baseVoltage = data.baseVoltage;
	responseStrength = data.responseStrength;

	m_dead = data.dead;
	m_frequency = data.frequency;
	m_voltage = data.voltage;

	for (const FGuid& guid : data.generators) {

		PW_ASSERT(buildingsMap.Contains(guid), LogSaveSubsystem, TEXT("Loaded buildings map does not contain guid '%s'"), *guid.ToString());

		AGenerator* generator = Cast<AGenerator>(*buildingsMap.Find(guid));
		PW_ASSERT(generator != nullptr, LogSaveSubsystem, TEXT("Building with guid '%s' was not serialized.."), *guid.ToString());

		m_generators.Add(generator);
		generator->SetNetwork(this);

	}
	for (const FGuid& guid : data.loads) {

		PW_ASSERT(buildingsMap.Contains(guid), LogSaveSubsystem, TEXT("Loaded buildings map does not contain guid '%s'"), *guid.ToString());

		ALoad* load = Cast<ALoad>(*buildingsMap.Find(guid));
		PW_ASSERT(load != nullptr, LogSaveSubsystem, TEXT("Building with guid '%s' was not serialized.."), *guid.ToString());

		m_loads.Add(load);
		load->SetNetwork(this);

	}
	for (const FGuid& guid : data.misc) {

		PW_ASSERT(buildingsMap.Contains(guid), LogSaveSubsystem, TEXT("Loaded buildings map does not contain guid '%s'"), *guid.ToString());
		
		ABuildInstance* miscBuildInstance = *buildingsMap.Find(guid);
		PW_ASSERT(miscBuildInstance != nullptr, LogSaveSubsystem, TEXT("Building with guid '%s' was not serialized."), *guid.ToString());

		m_miscBuildInstances.Add(miscBuildInstance);
		miscBuildInstance->SetNetwork(this);

	}

	for (const FGuid& guid : data.connections) {

		PW_ASSERT(buildingsMap.Contains(guid), LogSaveSubsystem, TEXT("Loaded buildings map does not contain guid '%s'"), *guid.ToString());

		AWire* wire = Cast<AWire>(*buildingsMap.Find(guid));
		PW_ASSERT(wire != nullptr, LogSaveSubsystem, TEXT("Building with guid '%s' was not serialized.."), *guid.ToString());

		m_connections.Add(wire);

		ABuildInstance* buildInstanceA = *buildingsMap.Find(wire->m_startGUID);
		ABuildInstance* buildInstanceB = *buildingsMap.Find(wire->m_endGUID);
		PW_ASSERT(buildInstanceA != nullptr && buildInstanceB != nullptr, LogSaveSubsystem, TEXT("Invalid start or end GUID on deserialized wire."));

		wire->SetNetwork(this);
		wire->m_startBuildInstance = buildInstanceA;
		wire->m_endBuildInstance = buildInstanceB;

		buildInstanceA->ConnectWire(wire);
		buildInstanceB->ConnectWire(wire);

	}

}

APowerNetwork* APowerNetwork::HandleConnection(ABuildInstance* buildInstanceA, ABuildInstance* buildInstanceB, AWire* wire) {

	PW_ASSERT(buildInstanceA != nullptr && buildInstanceB != nullptr, LogPower, TEXT("Can't handle connection between invalid build instances."));

	APowerNetwork* networkA = buildInstanceA->m_powerNetwork;
	APowerNetwork* networkB = buildInstanceB->m_powerNetwork;

	APowerNetwork* resultingNetwork = nullptr;
	if (networkA != nullptr && networkB != nullptr) {

		if (networkA == networkB) {

			networkA->m_connections.Add(wire);
			return networkA;

		}

		// Case 1: 2 separete networks merging into one.
		// TODO: Implement proper merging (changing values, handling load, etc)

		APowerNetwork* oldNetwork = nullptr;
		if (networkA->m_connections.Num() >= networkB->m_connections.Num()) {

			resultingNetwork = networkA;
			oldNetwork = networkB;

		} else {

			resultingNetwork = networkB;
			oldNetwork = networkA;

		}

		// Reassign all buildings in the smaller network to the new one.
		// There will be repeated assignements, but in the end those are faster than to
		// check if the network was already assigned each and every time.

		for (TObjectPtr<AWire> connection : oldNetwork->m_connections) {

			connection->m_powerNetwork = resultingNetwork;
			connection->m_startBuildInstance->m_powerNetwork = resultingNetwork;
			connection->m_endBuildInstance->m_powerNetwork = resultingNetwork;

			resultingNetwork->AddBuildInstance(connection->m_startBuildInstance);
			resultingNetwork->AddBuildInstance(connection->m_endBuildInstance);
			resultingNetwork->m_connections.Add(connection);

		}

		// Finally, destroy the old network

		oldNetwork->Destroy();

	} else if (networkA == nullptr && networkB == nullptr) {

		// Case 2: 2 unconnected buildings, new network is created.

		resultingNetwork = buildInstanceA->GetWorld()->SpawnActor<APowerNetwork>();
		PW_ASSERT(resultingNetwork != nullptr, LogPower, TEXT("Could not spawn new power network."));

	} else
		// Case 3: 1 connected, 1 unconnected building, the existing network is used.
		resultingNetwork = networkA != nullptr ? networkA : networkB;

	resultingNetwork->AddBuildInstance(buildInstanceA);
	resultingNetwork->AddBuildInstance(buildInstanceB);

	resultingNetwork->m_connections.Add(wire);

	return resultingNetwork;

}

void APowerNetwork::AddBuildInstance(ABuildInstance* buildInstance) {

	if (AGenerator* generator = Cast<AGenerator>(buildInstance)) {

		m_generators.Add(generator);
		generator->SetNetwork(this);

		if (m_generators.Num() == 1) {

			m_dead = false;
			m_frequency = baseFrequency;
			m_voltage = baseVoltage;

		}

	} else if (ALoad* load = Cast<ALoad>(buildInstance)) {

		m_loads.Add(load);
		load->SetNetwork(this);

	} else {

		m_miscBuildInstances.Add(buildInstance);
		buildInstance->SetNetwork(this);

	}

}