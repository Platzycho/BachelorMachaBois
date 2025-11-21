// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteWidgetBase.h"
#include "Components/CanvasPanelSlot.h"

UNoteWidgetBase::UNoteWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UNoteWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UNoteWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float TimeBeforeHit = TimeSeconds - CurrentTime;

	// Notes scroll downward (top → bottom)
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(this->Slot))
	{
		// Where the note should be hit (bottom of lane)
		const float JudgeLineY = 500.f;  // You can expose this later

		FVector2D Pos = CanvasSlot->GetPosition();

		// Move note from above toward JudgeLineY
		Pos.Y = JudgeLineY - (TimeBeforeHit * ScrollSpeed);

		CanvasSlot->SetPosition(Pos);
	}
}

void UNoteWidgetBase::InitializeFromEvent(float InTimeSeconds, float InDuration, int32 InNoteNumber, int32 InInputType)
{
	TimeSeconds = InTimeSeconds;
	DurationSeconds = InDuration;
	NoteNumber = InNoteNumber;
	InputType = InInputType;

	bIsHoldNote = (InDuration > 0.01f);
}