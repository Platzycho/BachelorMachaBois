// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoteWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BACHELORMACHABOIS_API UNoteWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UNoteWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Called by lane to initialize this note */
	UFUNCTION(BlueprintCallable)
	void InitializeFromEvent(float InTimeSeconds, float InDuration, int32 InNoteNumber, int32 InInputType);

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	float TimeSeconds = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	float DurationSeconds = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	int32 NoteNumber = 0;

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	int32 InputType = 0;

	UPROPERTY(BlueprintReadOnly, Category = "MIDI")
	bool bIsHoldNote = false;

protected:
	/** Movement speed (pixels per second – tune later) */
	UPROPERTY(EditAnywhere, Category = "Rhythm")
	float ScrollSpeed = 500.f;
};