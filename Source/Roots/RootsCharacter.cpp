// Copyright Epic Games, Inc. All Rights Reserved.

#include "RootsCharacter.h"
#include "RootsProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RootsGameState.h"
#include <Kismet/KismetMathLibrary.h>


//////////////////////////////////////////////////////////////////////////
// ARootsCharacter

ARootsCharacter::ARootsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;


	PrimaryActorTick.bCanEverTick = true;

	PivotSpeed = 10.f;

	TargetPivotDistance = 400.f;
	TimeToAdjustDistance = 1.f;
	TimeSpentAdjusting = 0.f;
}

void ARootsCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->bEnableClickEvents = true;
	}

}

void ARootsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeSpentAdjusting += DeltaTime;
	if (bIsFocussed && Controller != nullptr && Controller->IsPlayerController() && FocusComponent.IsValid())
	{
		FVector ComponentLocation = FocusComponent->GetComponentLocation();
		if (UBranchSegmentCPP* AsSegment = Cast<UBranchSegmentCPP>(FocusComponent))
		{
			ComponentLocation = AsSegment->GetEndLocation();
		}
		FVector OurLocation = GetActorLocation();
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(OurLocation, ComponentLocation);
		Cast<APlayerController>(Controller)->SetControlRotation(NewRotation);
		float SquareDistanceFromTarget = FVector::DistSquared(OurLocation, ComponentLocation);
		FVector TargetLocation = ComponentLocation + (NewRotation.RotateVector(FVector(1,0,0)) * (-TargetPivotDistance));
		SetActorLocation(FMath::Lerp(OurLocation, TargetLocation, FMath::Clamp(TimeSpentAdjusting / TimeToAdjustDistance, 0, 1)));
	}
	bool bIsMoving = false;
	if (Controller->IsPlayerController())
	{
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (UEnhancedPlayerInput* EnhancedInputComponent = Cast<UEnhancedPlayerInput>(PC->PlayerInput))
	{
		bIsMoving = EnhancedInputComponent->GetActionValue(MoveAction).GetMagnitudeSq() > 0.05f;
	}
	if ((bIsFocussed && !bIsPivoting) || (!bIsFocussed && !bIsMoving))
	{
		PC->bShowMouseCursor = true;
	}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void ARootsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARootsCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARootsCharacter::Look);
		EnhancedInputComponent->BindAction(PivotAction, ETriggerEvent::Triggered, this, &ARootsCharacter::StartPivot);
		EnhancedInputComponent->BindAction(PivotAction, ETriggerEvent::Completed, this, &ARootsCharacter::EndPivot);
		EnhancedInputComponent->BindAction(ResetFocusAction, ETriggerEvent::Triggered, this, &ARootsCharacter::ResetFocusToFirstRoots);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ARootsCharacter::Zoom);

		//Interacting
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &ARootsCharacter::TrySelect);
		EnhancedInputComponent->BindAction(ControlDirectionAction, ETriggerEvent::Triggered, this, &ARootsCharacter::StartControlling);
		EnhancedInputComponent->BindAction(ControlDirectionAction, ETriggerEvent::Completed, this, &ARootsCharacter::StopControlling);

	}
}
UBranchSegmentCPP* ARootsCharacter::GetSelectedBranchSegment() const
{
	return SelectedBranchSegment.IsValid() ? SelectedBranchSegment.Get() : nullptr;
}

void ARootsCharacter::SelectBranchSegment(UBranchSegmentCPP* Segment)
{
	if (SelectedBranchSegment.IsValid())
	{
		SelectedBranchSegment->OnUnselected();
	}
	SelectedBranchSegment = Segment;
	SelectedBranchSegment->OnSelected();
	SetViewTargetComponent(Segment);
}

void ARootsCharacter::SetViewTargetComponent(USceneComponent* NewFocusComponent)
{
	TimeSpentAdjusting = 0.f;
	FocusComponent = NewFocusComponent;
	bIsFocussed = true;
	TargetPivotDistance = 400.f;
	GetCharacterMovement()->BrakingDecelerationFlying = 200.f;
}

void ARootsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetViewRotation().Vector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
		FocusComponent.Reset();
		bIsFocussed = false;
		if (Controller->IsPlayerController())
		{
			APlayerController* PC = Cast<APlayerController>(Controller);
			PC->bShowMouseCursor = false;
			GetCharacterMovement()->BrakingDecelerationFlying = 1000.f;
		}
	}
}

void ARootsCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		bool bIsMoving = false;
		if (Controller->IsPlayerController())
		{
			if (UEnhancedPlayerInput* EnhancedInputComponent = Cast<UEnhancedPlayerInput>(Cast<APlayerController>(Controller)->PlayerInput))
			{
				bIsMoving = EnhancedInputComponent->GetActionValue(MoveAction).GetMagnitudeSq() > 0.05f;
			}
		}
		if (!bIsFocussed)
		{
			if (bIsMoving || bIsPivoting)
			{
				// add yaw and pitch input to controller
				AddControllerYawInput(LookAxisVector.X);
				AddControllerPitchInput(LookAxisVector.Y);
			}
		}
		else
		{
			if (bIsPivoting)
			{
				FVector DesiredMove = FVector(0, LookAxisVector.X * PivotSpeed, -(LookAxisVector.Y * PivotSpeed));
				FVector LocalMove = GetViewRotation().Quaternion() * DesiredMove;
				AddActorWorldOffset(LocalMove);
				if (Controller != nullptr && Controller->IsPlayerController())
				{
					APlayerController* PC = Cast<APlayerController>(Controller);
					PC->SetMouseLocation(CachedMousePosition.X, CachedMousePosition.Y);
				}
			}
			else if (bIsControllingDirection)
			{
				SelectedBranchSegment->CalculateNewDirection(FVector(0, -LookAxisVector.X, LookAxisVector.Y), GetActorLocation(), GetViewRotation());
			}
		}
	}
}

void ARootsCharacter::StartPivot(const FInputActionValue& Value)
{
	if (!bIsPivoting && Controller != nullptr && Controller->IsPlayerController())
	{
		float MouseX;
		float MouseY;
		APlayerController* PC = Cast<APlayerController>(Controller);
		PC->GetMousePosition(MouseX, MouseY);
		PC->bShowMouseCursor = false;
		CachedMousePosition = FIntVector(MouseX, MouseY, 0);
		UE_LOG(LogTemp, Warning, TEXT("StartingPivot"));
	}
	bIsPivoting = true;
}

void ARootsCharacter::EndPivot(const FInputActionValue& Value)
{
	bIsPivoting = false;
	if (Controller != nullptr && Controller->IsPlayerController())
	{
		APlayerController* PC = Cast<APlayerController>(Controller);
		PC->SetMouseLocation(CachedMousePosition.X, CachedMousePosition.Y);
		PC->bShowMouseCursor = true;
	}
}

void ARootsCharacter::ResetFocusToFirstRoots(const FInputActionValue& Value)
{
	ARootsGameState* const GameState = GetWorld()->GetGameState<ARootsGameState>();
	if (IsValid(GameState))
	{
		TArray<TWeakObjectPtr<ARootsSystemCPP>> Roots = GameState->GetRootsSystems();
		for (TWeakObjectPtr<ARootsSystemCPP> Root : Roots)
		{
			if (Root.IsValid())
			{
				SetViewTargetComponent(Root->RootBranch);
				return;
			}
		}
	}
}

void ARootsCharacter::TrySelect(const FInputActionValue& Value)
{
	if (Controller == nullptr || !Controller->IsPlayerController())
	{
		return;
	}
	APlayerController* PC = Cast<APlayerController>(Controller);
	FHitResult Result;
	PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2), false, Result);
	if (Result.IsValidBlockingHit())
	{
		if (UBranchSegmentCPP* Segment = Cast<UBranchSegmentCPP>(Result.GetComponent()->GetAttachParent()))
		{
			SelectBranchSegment(Segment);
			return;
		}
	}
}

void ARootsCharacter::StartControlling(const FInputActionValue& Value)
{
	bIsControllingDirection = true;
}

void ARootsCharacter::StopControlling(const FInputActionValue& Value)
{
	bIsControllingDirection = false;
}

void ARootsCharacter::Zoom(const FInputActionValue& Value)
{
	if (!bIsFocussed)
	{
		return;
	}
	// input is a Vector2D
	FVector2D ZoomVector = Value.Get<FVector2D>();
	TargetPivotDistance = FMath::Clamp(TargetPivotDistance + (50 * (ZoomVector.X < 0 ?  1 : -1)), 200, 1500);
}
