// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MidiReader.h"
#include "NoteWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "LaneWidgetBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EHitQuality : uint8
{
	Miss     UMETA(DisplayName = "Miss"),
	Good     UMETA(DisplayName = "Good"),
	Great     UMETA(DisplayName = "Great"),
	Perfect  UMETA(DisplayName = "Perfect")
};

UCLASS()
class BACHELORMACHABOIS_API ULaneWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	ULaneWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SpawnNote(const FMidiNoteEvent& Event);

	UFUNCTION(BlueprintCallable, Category = "Rhythm|Hit")
	EHitQuality TryHit(float CurrentTime, float PerfectWindow = 0.05f, float GreatWindow = 0.08f, float GoodWindow = 0.12f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notes")
	TSubclassOf<UNoteWidgetBase> NoteWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* NoteCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane")
	int32 LaneInputType = 0;

	TArray<UNoteWidgetBase*> ActiveNotes;

	UNoteWidgetBase* GetClosestNoteToHit(float CurrentTime, float HitWindow);
};