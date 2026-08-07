#include "OreVein.h"

#include "Player/MainPlayerCharacter.h"

#include "Inventory/InventoryComponent.h"

#include <Components/StaticMeshComponent.h>

AOreVein::AOreVein() {

	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(mesh);

}

void AOreVein::Tick(float deltaTime) {

	if (mineTimer > 0.0f)
		mineTimer -= deltaTime;

}

void AOreVein::Interact(AMainPlayerCharacter* character) {

	if (mineTimer <= 0.0f) {

		UInventoryComponent* inventory = character->GetInventoryComponent();
		inventory->AddItem(ore, yield);

		mineTimer = mineDuration;

	}

}