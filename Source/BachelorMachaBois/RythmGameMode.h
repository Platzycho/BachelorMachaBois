// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RythmGameMode.generated.h"

/**
 * 
 */
class UMidiReader;

UCLASS()
class BACHELORMACHABOIS_API ARythmGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public: 
	ARythmGameMode();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rhythm")
	UMidiReader* MidiReader;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rhythm")
	float ElapsedTime = 0.f;
};
