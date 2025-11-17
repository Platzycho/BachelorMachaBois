// Fill out your copyright notice in the Description page of Project Settings.


#include "LaneWidgetBase.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"

ULaneWidgetBase::ULaneWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void ULaneWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (!NoteCanvas)
	{
		UE_LOG(LogTemp, Error, TEXT("LaneWidgetBase: NoteCanvas is not bound!"));
	}
}

void ULaneWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	for (int32 i = ActiveNotes.Num() - 1; i >= 0; --i)
	{
		UNoteWidgetBase* Note = ActiveNotes[i];
		if (!Note) { ActiveNotes.RemoveAt(i); continue; }

		if (CurrentTime - Note->TimeSeconds > 1.0f)
		{
			Note->RemoveFromParent();
			ActiveNotes.RemoveAt(i);
		}
	}
}

void ULaneWidgetBase::SpawnNote(const FMidiNoteEvent& Event)
{
	if (!NoteWidgetClass || !NoteCanvas)
	{
		UE_LOG(LogTemp, Warning, TEXT("LaneWidgetBase::SpawnNote missing NoteWidgetClass or NoteCanvas"));
		return;
	}

	UNoteWidgetBase* Widget = CreateWidget<UNoteWidgetBase>(GetWorld(), NoteWidgetClass);
	if (!Widget) return;

	Widget->InitializeFromEvent(Event.TimeSeconds, Event.DurationSeconds, Event.NoteNumber, (int32)Event.InputType);

	NoteCanvas->AddChild(Widget);

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Widget->Slot))
	{
		CanvasSlot->SetAutoSize(true);
		CanvasSlot->SetPosition(FVector2D(0.f, 0.f));
	}

	ActiveNotes.Add(Widget);

	UE_LOG(LogTemp, Log, TEXT("Spawned note %d on lane %d at %.2f"), Event.NoteNumber, LaneInputType, Event.TimeSeconds);
}

UNoteWidgetBase* ULaneWidgetBase::GetClosestNoteToHit(float CurrentTime, float HitWindow)
{
	UNoteWidgetBase* BestNote = nullptr;
	float BestDistance = HitWindow;

	for (UNoteWidgetBase* Note : ActiveNotes)
	{
		if (!Note) continue;

		float Dist = FMath::Abs(Note->TimeSeconds - CurrentTime);
		if (Dist < BestDistance)
		{
			BestDistance = Dist;
			BestNote = Note;
		}
	}

	return BestNote;
}