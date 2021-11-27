#include "ConnectionDrawingPolicy_InstantDialogue.h"

#include "AssetGraphSchema_InstantDialogue.h"
#include "Nodes/EdNode_InstantDialogueNode.h"
#include "EdNode_InstantDialogueEdge.h"


////////////////////////////////////////////// Flow Graph
// FConnectionDrawingPolicy* FInstantDialogueConnectionDrawingPolicyFactory::CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
// {
// 	if (Schema->IsA(UAssetGraphSchema_InstantDialogue::StaticClass()))
// 	{
// 		return new FConnectionDrawingPolicy_InstantDialogue(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
// 	}
// 	return nullptr;
// }
/////////////////////////////////////////////////////// end

FConnectionDrawingPolicy_InstantDialogue::FConnectionDrawingPolicy_InstantDialogue(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj)
	: FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements)
	, GraphObj(InGraphObj)
{
}

//void FConnectionDrawingPolicy_InstantDialogue::BuildPaths()
//{
	/*if (const UInstantDialogue* InstantDialogue = CastChecked<UInstantDialogue>(GraphObj)->GetFlowAsset()->GetInspectedInstance())
	{
		const double CurrentTime = FApp::GetCurrentTime();

		for (const UInstantDialogueNode* Node : InstantDialogue->GetRecordedNodes())
		{
			const UEdNode_InstantDialogueNode* InstantDialogueNode = Cast<UEdNode_GInstantDialogueNode>(Node->GetGraphNode());

			for (const TPair<uint8, FInstantDialoguePinRecord>& Record : Node->GetWireRecords())
			{
				if (UEdGraphPin* OutputPin = InstantDialogueNode->OutputPins[Record.Key])
				{
					// check if Output pin is connected to anything
					if (OutputPin->LinkedTo.Num() > 0)
					{
						RecordedPaths.Emplace(OutputPin, OutputPin->LinkedTo[0]);

						if (CurrentTime < Record.Value.Time + RecentWireDuration)
						{
							RecentPaths.Emplace(OutputPin, OutputPin->LinkedTo[0]);
						}
					}
				}
			}
		}
	}*/

	/*if (GraphObj && (UFlowGraphSettings::Get()->bHighlightInputWiresOfSelectedNodes || UFlowGraphSettings::Get()->bHighlightOutputWiresOfSelectedNodes))
	{
		const TSharedPtr<FFlowAssetEditor> FlowAssetEditor = FFlowGraphUtils::GetFlowAssetEditor(GraphObj);
		if (FlowAssetEditor.IsValid())
		{
			for (UFlowGraphNode* SelectedNode : FlowAssetEditor->GetSelectedFlowNodes())
			{
				for (UEdGraphPin* Pin : SelectedNode->Pins)
				{
					if ((Pin->Direction == EGPD_Input && UFlowGraphSettings::Get()->bHighlightInputWiresOfSelectedNodes)
						|| (Pin->Direction == EGPD_Output && UFlowGraphSettings::Get()->bHighlightOutputWiresOfSelectedNodes))
					{
						for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
						{
							SelectedPaths.Emplace(Pin, LinkedPin);
						}
					}
				}
			}
		}
	}*/
//}

// used for the preview view
void FConnectionDrawingPolicy_InstantDialogue::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, /*inout*/ FConnectionParams& Params)
{
	// Params.AssociatedPin1 = OutputPin;
	// Params.AssociatedPin2 = InputPin;
	// Params.WireThickness = 2.5f;
	//
	// const bool bDeemphasizeUnhoveredPins = HoveredPins.Num() > 0;
	// if (bDeemphasizeUnhoveredPins)
	// {
	// 	ApplyHoverDeemphasis(OutputPin, InputPin, /*inout*/ Params.WireThickness, /*inout*/ Params.WireColor);
	// }

	//new
	Params.AssociatedPin1 = OutputPin;
	Params.AssociatedPin2 = InputPin;


	// //Flow Graph
	// ////////////////////////////////////////////////////////////////
	// Params.AssociatedPin1 = OutputPin;
	// Params.AssociatedPin2 = InputPin;
	//
	// // Get the schema and grab the default color from it
	// check(OutputPin);
	// check(GraphObj);
	// const UEdGraphSchema* Schema = GraphObj->GetSchema();
	//
	// if (OutputPin->bOrphanedPin || (InputPin && InputPin->bOrphanedPin))
	// {
	// 	Params.WireColor = FLinearColor::Red;
	// }
	// else
	// {
	// 	Params.WireColor = Schema->GetPinTypeColor(OutputPin->PinType);
	//
	// 	if (InputPin)
	// 	{
	// 		// selected paths
	// 		if (SelectedPaths.Contains(OutputPin) || SelectedPaths.Contains(InputPin))
	// 		{
	// 			Params.WireColor = FLinearColor::Green;
	// 			Params.WireThickness = 2.0f;
	// 			Params.bDrawBubbles = false;
	// 			return;
	// 		}
	//
	// 		// recent paths
	// 		if (RecentPaths.Contains(OutputPin) && RecentPaths[OutputPin] == InputPin)
	// 		{
	// 			Params.WireColor = FLinearColor::Blue;
	// 			Params.WireThickness = 2.5f;
	// 			Params.bDrawBubbles = true;
	// 			return;
	// 		}
	//
	// 		// all paths, showing graph history
	// 		if (RecordedPaths.Contains(OutputPin) && RecordedPaths[OutputPin] == InputPin)
	// 		{
	// 			Params.WireColor = FLinearColor::Yellow;
	// 			Params.WireThickness = 3.5f;
	// 			Params.bDrawBubbles = false;
	// 			return;
	// 		}
	//
	// 		// It's not followed, fade it and keep it thin
	// 		Params.WireColor = FLinearColor::White;
	// 		Params.WireThickness = 2.5f;
	// 	}
	// }
}

void FConnectionDrawingPolicy_InstantDialogue::Draw(TMap<TSharedRef<SWidget>, FArrangedWidget>& InPinGeometries, FArrangedChildren& ArrangedNodes)
{
	// Build an acceleration structure to quickly find geometry for the nodes
	NodeWidgetMap.Empty();
	for (int32 NodeIndex = 0; NodeIndex < ArrangedNodes.Num(); ++NodeIndex)
	{
		FArrangedWidget& CurWidget = ArrangedNodes[NodeIndex];
		TSharedRef<SGraphNode> ChildNode = StaticCastSharedRef<SGraphNode>(CurWidget.Widget);
		NodeWidgetMap.Add(ChildNode->GetNodeObj(), NodeIndex);
	}

	// Now draw
	FConnectionDrawingPolicy::Draw(InPinGeometries, ArrangedNodes); //this draws the line

	//Flow Graph one
	////////////////////////////////////////////
	//BuildPaths();
	//FConnectionDrawingPolicy::Draw(InPinGeometries, ArrangedNodes);
}

void FConnectionDrawingPolicy_InstantDialogue::DrawPreviewConnector(const FGeometry& PinGeometry, const FVector2D& StartPoint, const FVector2D& EndPoint, UEdGraphPin* Pin)
{
	// FConnectionParams Params;
	// DetermineWiringStyle(Pin, nullptr, /*inout*/ Params);
	//
	// if (Pin->Direction == EEdGraphPinDirection::EGPD_Output)
	// {
	// 	DrawSplineWithArrow(FGeometryHelper::FindClosestPointOnGeom(PinGeometry, EndPoint), EndPoint, Params);
	// }
	// else
	// {
	// 	DrawSplineWithArrow(FGeometryHelper::FindClosestPointOnGeom(PinGeometry, StartPoint), StartPoint, Params);
	// }


	//new stuff
	FConnectionParams Params;
	DetermineWiringStyle(Pin, nullptr, /*inout#*/ Params);

	DrawSplineWithArrow(StartPoint, EndPoint, Params);
	
}

// void FConnectionDrawingPolicy_InstantDialogue::DrawSplineWithArrow(const FVector2D& StartAnchorPoint, const FVector2D& EndAnchorPoint, const FConnectionParams& Params)
// {
// 	// bUserFlag1 indicates that we need to reverse the direction of connection (used by debugger)
// 	const FVector2D& P0 = Params.bUserFlag1 ? EndAnchorPoint : StartAnchorPoint;
// 	const FVector2D& P1 = Params.bUserFlag1 ? StartAnchorPoint : EndAnchorPoint;
//
// 	//Internal_DrawLineWithArrow(P0, P1, Params); //this one differs from the other script
// }

// stolen one <-------------- start
void FConnectionDrawingPolicy_InstantDialogue::DrawSplineWithArrow(const FVector2D& StartPoint, const FVector2D& EndPoint, const FConnectionParams& Params)
{
	// Draw the spline
	DrawConnection(
		WireLayerID,
		StartPoint,
		EndPoint,
		Params);

	// Draw the arrow
	if (ArrowImage != nullptr)
	{
		FVector2D ArrowPoint = EndPoint - ArrowRadius;

		FSlateDrawElement::MakeBox(
			DrawElementsList,
			ArrowLayerID,
			FPaintGeometry(ArrowPoint, ArrowImage->ImageSize * ZoomFactor, ZoomFactor),
			ArrowImage,
			ESlateDrawEffect::None,
			Params.WireColor
			);
	}
}
// stolen one <-------------- end
void FConnectionDrawingPolicy_InstantDialogue::DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom, const FConnectionParams& Params) //this is the code for the constant spline/edge
{
	// Get a reasonable seed point (halfway between the boxes)
	//const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	//const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	//const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	// Find the (approximate) closest points between the two boxes
	//const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	//const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	//DrawSplineWithArrow(StartAnchorPoint, EndAnchorPoint, Params);

	
	//new one the Y value is hardcoded (don't know why the Spline is over the Pins)
	const float StartFudgeX = 4.0f; //4.0f
	const float EndFudgeX = 4.0f; //4.0f
	const FVector2D StartPoint = FGeometryHelper::VerticalMiddleRightOf(StartGeom) - FVector2D(StartFudgeX, -00.0f);
	const FVector2D EndPoint = FGeometryHelper::VerticalMiddleLeftOf(EndGeom) - FVector2D(ArrowRadius.X - EndFudgeX, -00.0f);
	
	DrawSplineWithArrow(StartPoint, EndPoint, Params);
}


void FConnectionDrawingPolicy_InstantDialogue::Internal_DrawLineWithArrow(const FVector2D& StartAnchorPoint, const FVector2D& EndAnchorPoint, const FConnectionParams& Params)
{
	//@TODO: Should this be scaled by zoom factor?
	const float LineSeparationAmount = 4.5f;

	const FVector2D DeltaPos = EndAnchorPoint - StartAnchorPoint;
	const FVector2D UnitDelta = DeltaPos.GetSafeNormal();
	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();

	// Come up with the final start/end points
	const FVector2D DirectionBias = Normal * LineSeparationAmount;
	const FVector2D LengthBias = ArrowRadius.X * UnitDelta;
	const FVector2D StartPoint = StartAnchorPoint + DirectionBias + LengthBias;
	const FVector2D EndPoint = EndAnchorPoint + DirectionBias - LengthBias;

	// Draw a line/spline, function in real one -----isn't responsible for the shape
	DrawConnection(WireLayerID, StartPoint, EndPoint, Params);

	// Draw the arrow
	const FVector2D ArrowDrawPos = EndPoint - ArrowRadius;
	const float AngleInRadians = FMath::Atan2(DeltaPos.Y, DeltaPos.X);

	FSlateDrawElement::MakeRotatedBox(
		DrawElementsList,
		ArrowLayerID,
		FPaintGeometry(ArrowDrawPos, ArrowImage->ImageSize * ZoomFactor, ZoomFactor),
		ArrowImage,
		ESlateDrawEffect::None,
		AngleInRadians,
		TOptional<FVector2D>(),
		FSlateDrawElement::RelativeToElement,
		Params.WireColor
	);
}

FVector2D FConnectionDrawingPolicy_InstantDialogue::ComputeSplineTangent(const FVector2D& Start, const FVector2D& End) const //drawing logic
{
	//const FVector2D Delta = End - Start;
	//const FVector2D NormDelta = Delta.GetSafeNormal();

	//return NormDelta;

	//returns the spline, so it can be drawn
	return Settings->ComputeSplineTangent(Start, End);
}

void FConnectionDrawingPolicy_InstantDialogue::DetermineLinkGeometry(FArrangedChildren& ArrangedNodes, TSharedRef<SWidget>& OutputPinWidget,
 	UEdGraphPin* OutputPin, UEdGraphPin* InputPin, FArrangedWidget*& StartWidgetGeometry, FArrangedWidget*& EndWidgetGeometry)
{

	//buged out the permanent line (wasn't visible)
	/*StartWidgetGeometry = PinGeometries->Find(OutputPinWidget);
	
	if (TSharedPtr<SGraphPin>* pTargetWidget = PinToPinWidgetMap.Find(InputPin))
	{
		TSharedRef<SGraphPin> InputWidget = (*pTargetWidget).ToSharedRef();
		EndWidgetGeometry = PinGeometries->Find(InputWidget);
	}*/


	if (UEdNode_InstantDialogueEdge* EdgeNode = Cast<UEdNode_InstantDialogueEdge>(InputPin->GetOwningNode()))
	{
		UEdNode_InstantDialogueNode* Start = EdgeNode->GetStartNode();
		UEdNode_InstantDialogueNode* End = EdgeNode->GetEndNode();
		if (Start != nullptr && End != nullptr)
		{
			int32* StartNodeIndex = NodeWidgetMap.Find(Start);
			int32* EndNodeIndex = NodeWidgetMap.Find(End);
			if (StartNodeIndex != nullptr && EndNodeIndex != nullptr)
			{
				StartWidgetGeometry = &(ArrangedNodes[*StartNodeIndex]);
				EndWidgetGeometry = &(ArrangedNodes[*EndNodeIndex]);
			}
		}
	}
	else
	{
		StartWidgetGeometry = PinGeometries->Find(OutputPinWidget);

		if (TSharedPtr<SGraphPin>* pTargetWidget = PinToPinWidgetMap.Find(InputPin))
		{
			TSharedRef<SGraphPin> InputWidget = (*pTargetWidget).ToSharedRef();
			EndWidgetGeometry = PinGeometries->Find(InputWidget);
		}
	}
}