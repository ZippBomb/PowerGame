#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "Core/Core.h"

#include "BuildInstance.generated.h"

class UBuild;

class AWire;
class APowerNetwork;

// UE stuff

struct FInstancedStruct;

UCLASS(Abstract)
class POWERGAME_API ABuildInstance : public AActor {

	GENERATED_BODY()

	friend APowerNetwork;
	friend AWire;
	
public:
	ABuildInstance();

	UFUNCTION(BlueprintCallable)
	void SetBuild(UBuild* build, FGuid guid = FGuid());

	UFUNCTION(BlueprintCallable)
	virtual void ConnectWire(AWire* wire);

	UFUNCTION(BlueprintCallable)
	virtual void Deconstruct();

	// Serialization

	virtual void SerializeSaveData(FInstancedStruct* out);
	virtual void DeserializeSaveData(const FInstancedStruct& data);

	// Highlight material

	UFUNCTION(BlueprintCallable)
	void SetHighlightMaterial(UMaterialInterface* material);
	UFUNCTION(BlueprintCallable)
	void ResetHighlightMaterial();

	// Getters

	UFUNCTION(BlueprintCallable)
	inline UBuild* GetBuild() const { return m_build; }
	UFUNCTION(BlueprintCallable)
	inline FGuid& GetGUID() { return m_guid; }

	UFUNCTION(BlueprintCallable)
	inline APowerNetwork* GetNetwork() const { return m_powerNetwork; }

	// Setters

	inline void SetNetwork(APowerNetwork* network) { m_powerNetwork = network; }

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> mesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APowerNetwork> m_powerNetwork = nullptr;

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBuild> m_build = nullptr;
	UPROPERTY(VisibleAnywhere)
	FGuid m_guid;
	
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AWire>> m_connectedWires;
	
};
