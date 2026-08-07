#include "Player/MainPlayerCharacter.h"

#include "Interactible.h"

#include "Inventory/InventoryComponent.h"

#include "Building/ConstructionModeManager.h"

#include "Building/Instances/BuildInstance.h"

#include "UI/PauseMenu.h"

#include "Saving/WorldSaveData.h"

#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Camera/CameraComponent.h>

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <InputActionValue.h>

#include <Kismet/GameplayStatics.h>

DEFINE_LOG_CATEGORY(LogCharacter)

AMainPlayerCharacter::AMainPlayerCharacter() {

	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);

	// Camera setup

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(GetCapsuleComponent());
	camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
	camera->bUsePawnControlRotation = true;

	// Arm mesh setup

	armMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	armMesh->SetupAttachment(camera);
	armMesh->SetOnlyOwnerSee(true);
	armMesh->SetRelativeLocation(FVector(-30.0f, 0.0f, -150.0f));
	armMesh->bCastDynamicShadow = false;
	armMesh->CastShadow = false;

	// Inventory setup

	inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	// Build mode setup

	constructionModeManager = CreateDefaultSubobject<UConstructionModeManager>(TEXT("BuildModeManager"));

}

void AMainPlayerCharacter::Tick(float deltaTime) {

	Super::Tick(deltaTime);

	if (constructionModeManager->GetTool() != EConstructionTool::None) return;

	// Interaction trace

	FHitResult hit = FHitResult(EForceInit::ForceInit);

	FVector start = camera->GetComponentLocation();
	FVector end = start + (camera->GetForwardVector() * interactionRange);

	FCollisionQueryParams params = FCollisionQueryParams(TEXT("Construction mode visibility trace"), true, this);
	params.bReturnPhysicalMaterial = false;
	params.bDebugQuery = true;

	if (!GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, params)) {

		targetInteractible = nullptr;
		return;

	}

	// Get the interactible

	TObjectPtr<AInteractible> target = Cast<AInteractible>(hit.GetActor());
	if (target != nullptr)
		targetInteractible = target;
	else
		targetInteractible = nullptr;

}

void AMainPlayerCharacter::Move(const FInputActionValue& value) {

	FVector2D movementVector = value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), movementVector.Y);
	AddMovementInput(GetActorRightVector(), movementVector.X);

}
void AMainPlayerCharacter::Look(const FInputActionValue& value) {

	FVector2D lookVector = value.Get<FVector2D>();

	AddControllerYawInput(lookVector.X);
	AddControllerPitchInput(lookVector.Y);

}

void AMainPlayerCharacter::OpenPauseMenu(const FInputActionValue& value) {

	PW_ASSERT(m_pauseMenu != nullptr, LogCharacter, TEXT("AMainPlayerCharacter did not had a PauseMenu ui bound."));

	m_pauseMenu->Open();

}

void AMainPlayerCharacter::Interact(const FInputActionValue& value) {

	if (targetInteractible == nullptr || !targetInteractible->IsInteractible()) return;

	targetInteractible->Interact(this);

}


void AMainPlayerCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent) {

	Super::SetupPlayerInputComponent(playerInputComponent);

	UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(playerInputComponent);
	PW_ASSERT(inputComponent != nullptr, LogCharacter, TEXT("'%s' could not get the Enhanced input component."), *GetNameSafe(this));

	// Movement

	inputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &AMainPlayerCharacter::Move);
	inputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &AMainPlayerCharacter::Look);

	inputComponent->BindAction(jumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	inputComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	// Interactions

	inputComponent->BindAction(interactAction, ETriggerEvent::Triggered, this, &AMainPlayerCharacter::Interact);

	// Inventory

	// Pause menu

	inputComponent->BindAction(openPauseMenuAction, ETriggerEvent::Triggered, this, &AMainPlayerCharacter::OpenPauseMenu);

	// Mode switching

	inputComponent->BindAction(selectBuildToolAction, ETriggerEvent::Started, constructionModeManager.Get(), &UConstructionModeManager::SelectBuildTool);
	inputComponent->BindAction(selectDeconstructToolAction, ETriggerEvent::Started, constructionModeManager.Get(), &UConstructionModeManager::SelectDeconstructTool);

}
