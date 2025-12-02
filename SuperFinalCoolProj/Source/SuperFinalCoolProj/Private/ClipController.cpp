// Fill out your copyright notice in the Description page of Project Settings.


#include "ClipController.h"

ClipController::ClipController()
{
}

ClipController::~ClipController()
{
}

void ClipController::UpdateClipController(float dt)
{
	currClipTime += dt * currClip->RateScale; //Add reverse playback here too

	if (currClipTime > currClip->GetPlayLength()) 
	{
		//Handle looping
		currClipTime = 0;
	}

}
