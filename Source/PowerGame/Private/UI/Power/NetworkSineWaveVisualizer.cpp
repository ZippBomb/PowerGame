#include "UI/Power/NetworkSineWaveVisualizer.h"

#include "Power/PowerNetwork.h"

#include <Rendering/DrawElements.h>

int32 UNetworkSineWaveVisualizer::NativePaint(const FPaintArgs& args, const FGeometry& allotedGeometry, const FSlateRect& cullingRect, FSlateWindowElementList& out, int32 layer, const FWidgetStyle& widgetStyle, bool parentEnabled) const {

	Super::NativePaint(args, allotedGeometry, cullingRect, out, layer, widgetStyle, parentEnabled);
	DrawGrid(allotedGeometry, out, layer);

	// Calculate the frequency difference

	float frequencyDiff = 0.0f;

	if (showDebugWave)
		frequencyDiff = debugFrequency - 50.0f;
	else if (m_network != nullptr && !m_network->IsDead())
		frequencyDiff = m_network->GetFrequency() - m_network->GetNominalFrequency();
	else return layer;

	frequencyDiff *= amplification;
	frequencyDiff = FMath::Clamp(frequencyDiff, frequencyDiffMin, frequencyDiffMax);

	// Form the sine wave

	const FVector2D size = allotedGeometry.GetLocalSize();

	TArray<FVector2D> points;
	points.Reserve(numSamples);

	for (uint32 i = 0; i < numSamples; i++) {

		float t = (float)i / numSamples;
		float frequency = baseWaveCycles + frequencyDiff;

		float y = size.Y * 0.5f + FMath::Sin((t * frequency * PI * 2.0f) + m_phase) * amplitude;

		points.Add(FVector2D(t * size.X, y));

	}

	// Get the colour based on quality

	FLinearColor finalColor = normalQualityColor;
	if (frequencyDiff < 0.0f) {

		float t = FMath::GetMappedRangeValueUnclamped(FVector2D(0.0f, frequencyDiffMin), FVector2D(0.0f, 1.0f), frequencyDiff);
		finalColor = FLinearColor::LerpUsingHSV(normalQualityColor, lowQualityColor, t);
	
	} else if (frequencyDiff > 0.0f) {

		float t = FMath::GetMappedRangeValueUnclamped(FVector2D(0.0f, frequencyDiffMax), FVector2D(0.0f, 1.0f), frequencyDiff);
		finalColor = FLinearColor::LerpUsingHSV(normalQualityColor, highQualityColor, t);

	}

	// Render the sine wave itself

	FSlateDrawElement::MakeLines(out, layer + 1, allotedGeometry.ToPaintGeometry(), points, ESlateDrawEffect::None, finalColor, true, lineThickness);
	return layer + 1;

}
void UNetworkSineWaveVisualizer::NativeTick(const FGeometry& geometry, float deltaTime) {

	Super::NativeTick(geometry, deltaTime);

	m_phase += deltaTime * scrollSpeed;
	m_phase = FMath::Fmod(m_phase, PI * 2.0f);

}

void UNetworkSineWaveVisualizer::DrawGrid(const FGeometry& allotedGeometry, FSlateWindowElementList& out, int32& layer) const {

	TArray<FVector2D> points;
	points.SetNumUninitialized(2);

	const FVector2D size = allotedGeometry.GetLocalSize();
	layer++;

	float majorStep = size.X / majorGridSegments;
	for (float majorX = 0; majorX <= size.X; majorX += majorStep) {

		points[0] = FVector2D(majorX, 0.0f);
		points[1] = FVector2D(majorX, size.Y);

		FSlateDrawElement::MakeLines(out, layer, allotedGeometry.ToPaintGeometry(), points, ESlateDrawEffect::None, majorLineColor, false, gridLineThickness);

		for (uint32 i = 1; i < minorGridSegments; i++) {

			float minorX = majorX + majorStep / minorGridSegments * i;

			points[0] = FVector2D(minorX, 0.0f);
			points[1] = FVector2D(minorX, size.Y);

			FSlateDrawElement::MakeLines(out, layer + 1, allotedGeometry.ToPaintGeometry(), points, ESlateDrawEffect::None, minorLineColor, false, gridLineThickness);

		}

	}

	layer += 2;
	points[0] = FVector2D(0.0f, size.Y / 2.0f);
	points[1] = FVector2D(size.X, size.Y / 2.0f);

	FSlateDrawElement::MakeLines(out, layer, allotedGeometry.ToPaintGeometry(), points, ESlateDrawEffect::None, centerLineColor, false, gridLineThickness);

}