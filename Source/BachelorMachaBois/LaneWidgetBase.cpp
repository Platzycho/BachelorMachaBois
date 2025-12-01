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
		UE_LOG(LogTemp, Error, TEXT("LaneWidget: NoteCanvas NOT bound!"));
	}
}

void ULaneWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	// Remove notes after passing judge line
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

	// --- AUTO CORRECT NOTE SIZES WHEN GEOMETRY BECOMES VALID ---
	if (NoteCanvas)
	{
		float Width = NoteCanvas->GetCachedGeometry().GetLocalSize().X;

		if (Width > 5.f)
		{
			for (UNoteWidgetBase* Note : ActiveNotes)
			{
				if (!Note) continue;

				if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Note->Slot))
				{
					if (CanvasSlot->GetSize().X < 5.f)
					{
						CanvasSlot->SetSize(FVector2D(Width, 80.f));
						CanvasSlot->SetZOrder(10);
					}
				}
			}
		}
	}
}

void ULaneWidgetBase::SpawnNote(const FMidiNoteEvent& Event)
{
	if (!NoteWidgetClass || !NoteCanvas)
	{
		UE_LOG(LogTemp, Warning, TEXT("LaneWidget %d: Missing NoteWidgetClass or NoteCanvas"), LaneInputType);
		return;
	}

	UNoteWidgetBase* Widget = CreateWidget<UNoteWidgetBase>(GetWorld(), NoteWidgetClass);
	if (!Widget) return;

	Widget->InitializeFromEvent(Event.TimeSeconds, Event.DurationSeconds, Event.NoteNumber, (int32)Event.InputType);

	NoteCanvas->AddChild(Widget);

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Widget->Slot))
	{
		CanvasSlot->SetAutoSize(false);

		float Width = NoteCanvas->GetCachedGeometry().GetLocalSize().X;
		if (Width <= 1.f)
			Width = 120.f; // fallback

		CanvasSlot->SetSize(FVector2D(Width, 80.f));
		CanvasSlot->SetPosition(FVector2D(0.f, -120.f)); // spawn above lane
		CanvasSlot->SetZOrder(10);
	}

	ActiveNotes.Add(Widget);

	UE_LOG(LogTemp, Log, TEXT("Spawned note %d on lane %d at %.2f"),
		Event.NoteNumber, LaneInputType, Event.TimeSeconds);
}

EHitQuality ULaneWidgetBase::TryHit(float CurrentTime, float PerfectWindow, float GreatWindow, float GoodWindow)
{
	if (ActiveNotes.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Lane %d: TryHit -> no active notes"), LaneInputType);
		return EHitQuality::Miss;
	}

	UNoteWidgetBase* BestNote = nullptr;
	float BestDistance = GoodWindow; // we only care up to GoodWindow

	for (UNoteWidgetBase* Note : ActiveNotes)
	{
		if (!Note) continue;

		const float Dist = FMath::Abs(Note->TimeSeconds - CurrentTime);
		if (Dist < BestDistance)
		{
			BestDistance = Dist;
			BestNote = Note;
		}
	}

	if (!BestNote)
	{
		UE_LOG(LogTemp, Log, TEXT("Lane %d: TryHit -> no note within window"), LaneInputType);
		return EHitQuality::Miss;
	}

	// Decide quality by distance
	EHitQuality Quality = EHitQuality::Miss;
	if (BestDistance <= PerfectWindow)
	{
		Quality = EHitQuality::Perfect;
	}
	else if (BestDistance <= GreatWindow)
	{
		Quality = EHitQuality::Great;
	}
	else if (BestDistance <= GoodWindow)
	{
		Quality = EHitQuality::Good;
	}
	else
	{
		return EHitQuality::Miss;
	}

	// Remove the hit note from UI and list
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(BestNote->Slot))
	{
		// (optional: snap note to judge line or play animation before removal)
	}

	BestNote->RemoveFromParent();
	ActiveNotes.Remove(BestNote);

	UE_LOG(LogTemp, Log, TEXT("Lane %d: HIT %s, dist=%.3f"),
		LaneInputType,
		*UEnum::GetValueAsString(Quality),
		BestDistance);

	return Quality;
}

UNoteWidgetBase* ULaneWidgetBase::GetClosestNoteToHit(float CurrentTime, float HitWindow)
{
	UNoteWidgetBase* Best = nullptr;
	float BestDistance = HitWindow;

	for (UNoteWidgetBase* Note : ActiveNotes)
	{
		if (!Note) continue;

		float Dist = FMath::Abs(Note->TimeSeconds - CurrentTime);
		if (Dist < BestDistance)
		{
			BestDistance = Dist;
			Best = Note;
		}
	}

	return Best;
}