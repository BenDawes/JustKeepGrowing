// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FocusNub.h"
#include "RootsCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class ARootsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Focus Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* PivotAction;
	/** Focus Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ResetFocusAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* SelectAction;
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ZoomAction;


	/** Control Direction Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ControlDirectionAction;


	/** Look controls */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float PivotSpeed;

	/** Look controls */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float TargetPivotDistance;
	/** Look controls */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float TimeToAdjustDistance;
	float TimeSpentAdjusting;

	FIntVector CachedMousePosition;

public:
	ARootsCharacter();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

public:

	TWeakObjectPtr<USceneComponent> FocusComponent;

	TWeakObjectPtr<UBranchSegmentCPP> SelectedBranchSegment;

	UFUNCTION(BlueprintCallable)
		UBranchSegmentCPP* GetSelectedBranchSegment() const;


	UFUNCTION(BlueprintCallable)
		void SelectBranchSegment(UBranchSegmentCPP* Segment);

	UFUNCTION(BlueprintCallable)
		void SetViewTargetComponent(USceneComponent* NewFocusComponent);

	UPROPERTY()
		bool bIsFocussed = false;

	UPROPERTY()
		bool bIsPivoting = false;

	UPROPERTY()
		bool bIsControllingDirection = false;


protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void StartPivot(const FInputActionValue& Value);
	void EndPivot(const FInputActionValue& Value);
	void ResetFocusToFirstRoots(const FInputActionValue& Value);
	void TrySelect(const FInputActionValue& Value);
	void StartControlling(const FInputActionValue& Value);
	void StopControlling(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


};

