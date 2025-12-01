// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RhythmPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BACHELORMACHABOIS_API ARhythmPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ARhythmPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void OnHitUp();
	void OnHitDown();
	void OnHitLeft();
	void OnHitRight();

	void OnHitW();
	void OnHitA();
	void OnHitS();
	void OnHitD();

	void SendLaneInput(int32 LaneInputType);
	
};
