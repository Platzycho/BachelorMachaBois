// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteWidgetBase.h"
#include "Components/CanvasPanelSlot.h"

UNoteWidgetBase::UNoteWidgetBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // UUserWidget does NOT tick unless explicitly allowed
    //SetCanTick(true);
}

void UNoteWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    SpawnTime = GetWorld()->GetTimeSeconds();
}

// Movement logic executed every frame
void UNoteWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    const float CurrentTime = GetWorld()->GetTimeSeconds();
    const float TimeBeforeHit = TimeSeconds - CurrentTime;

    if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(this->Slot))
    {
        FVector2D Pos = CanvasSlot->GetPosition();
        Pos.Y = TimeBeforeHit * ScrollSpeed * -1.f;  // Move downward
        CanvasSlot->SetPosition(Pos);
    }

    // Later you’ll add:
    // - Despawning when below hit line
    // - Stretching for hold notes
    // - Hit window checking
}

// Called by lane when note is created
void UNoteWidgetBase::InitializeFromEvent(float InTimeSeconds, float InDuration, int32 InNoteNumber, int32 InInputType)
{
    TimeSeconds = InTimeSeconds;
    DurationSeconds = InDuration;
    NoteNumber = InNoteNumber;
    InputType = InInputType;

    bIsHoldNote = (InDuration > 0.01f);
}