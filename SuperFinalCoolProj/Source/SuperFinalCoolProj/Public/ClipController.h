// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SUPERFINALCOOLPROJ_API ClipController
{
public:
	ClipController();
	~ClipController();

	void UpdateClipController(float dt);

	UAnimSequence* currClip;
	float currClipTime;
	float currClipTimeNormalized;
};
