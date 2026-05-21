#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include <Logging/LogMacros.h>

#include "Core/Core.h"

#include "PowerNetwork.generated.h"

class ABuildInstance;

class AGenerator;
class ALoad;
class AWire;

struct FNetworkSaveData;

DECLARE_LOG_CATEGORY_EXTERN(LogPower, Log, All);

UCLASS()
class POWERGAME_API APowerNetwork : public AActor {

	GENERATED_BODY()
	
public:
	APowerNetwork();

	UFUNCTION(BlueprintCallable)
	void ConnectGenerator(AGenerator* generator);
	UFUNCTION(BlueprintCallable)
	void ConnectLoad(ALoad* load);

	UFUNCTION(BlueprintCallable)
	void DisconnectGenerator(AGenerator* generator);
	UFUNCTION(BlueprintCallable)
	void DisconnectLoad(ALoad* load);
	UFUNCTION(BlueprintCallable)
	void DisconnectWire(AWire* wire);

	void SerializeSaveData(FNetworkSaveData* out);
	void DeserializeSaveData(const FNetworkSaveData& data, TMap<FGuid, TObjectPtr<ABuildInstance>>& buildingsMap);

	UFUNCTION(BlueprintCallable)
	static APowerNetwork* HandleConnection(ABuildInstance* buildInstanceA, ABuildInstance* buildInstanceB, AWire* wire);

	UFUNCTION(BlueprintCallable)
	inline float GetBaseFrequency() const { return baseFrequency; }
	UFUNCTION(BlueprintCallable)
	inline float GetBaseVoltage() const { return baseVoltage; }

	UFUNCTION(BlueprintCallable)
	inline bool IsDead() const { return m_dead; }
	UFUNCTION(BlueprintCallable)
	inline float GetFrequency() const { return m_frequency; }
	UFUNCTION(BlueprintCallable)
	inline float GetVoltage() const { return m_voltage; }

protected:
	UPROPERTY(EditAnywhere)
	float baseFrequency = 50.0f;
	UPROPERTY(EditAnywhere)
	float baseVoltage = 230.0f;
	UPROPERTY(EditAnywhere)
	float responseStrength = 0.0005f;

private:
	UPROPERTY(VisibleAnywhere)
	bool m_dead = true;
	UPROPERTY(VisibleAnywhere)
	float m_voltage = 0.0f;
	UPROPERTY(VisibleAnywhere)
	float m_frequency = 0.0f;

	UPROPERTY(VisibleAnywhere)
	float m_totalSupply = 0.0f;
	UPROPERTY(VisibleAnywhere)
	float m_totalDemand = 0.0f;

	UPROPERTY(VisibleAnywhere)
	TSet<TObjectPtr<AGenerator>> m_generators;
	UPROPERTY(VisibleAnywhere)
	TSet<TObjectPtr<ALoad>> m_loads;
	UPROPERTY(VisibleAnywhere)
	TSet<TObjectPtr<ABuildInstance>> m_miscBuildInstances;
	UPROPERTY(VisibleAnywhere)
	TSet<TObjectPtr<AWire>> m_connections;

	virtual void Tick(float deltaTime) override;

	void AddBuildInstance(ABuildInstance* buildInstance);
	
};