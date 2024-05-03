#pragma once

#include "pch.h"
#include <cControlGameEngine.h>

enum class EasingType
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut
};

struct sPositionKeyFrame
{
	sPositionKeyFrame(const glm::vec3& position, double time, EasingType type = EasingType::Linear)
		: mPosition(position)
		, mTime(time)
		, mEaseType(type)
	{ }
	glm::vec3 mPosition;
	EasingType mEaseType;
	double mTime;
};

struct sScaleKeyFrame
{
	sScaleKeyFrame(const float& scale, double time, EasingType type = EasingType::Linear)
		: mScale(scale)
		, mTime(time)
		, mEaseType(type)
	{ }
	float mScale;
	EasingType mEaseType;
	double mTime;
};

struct sRotationKeyFrame
{
	sRotationKeyFrame(float angleDegrees, const glm::vec3& rotation, double time, EasingType type = EasingType::Linear)
		: mAngleDegrees(angleDegrees)
		, mRotation(rotation)
		, mTime(time)
		, mEaseType(type)
	{ }
	float mAngleDegrees;
	glm::vec3 mRotation;
	EasingType mEaseType;
	double mTime;
};

struct sKeyFrameEvent
{
	sKeyFrameEvent(bool timeBasedEvent, std::string modelName) 
		: mTime(0.f)
		, mModelName(modelName)
		, mEventTriggered(false) 
		, mTimeBasedEvent(timeBasedEvent)
		, mEaseType(EasingType::Linear)
	{ }
	double mTime;
	std::string mModelName;
	bool mEventTriggered;
	bool mTimeBasedEvent;
	EasingType mEaseType;
	
	bool checkForEventTrigger(std::string modelName, double animationTime, sPositionKeyFrame &positionKeyFrame, bool isReversed, cControlGameEngine &gameEngine)
	{
		if (modelName == mModelName)
		{
			if (mTimeBasedEvent)
			{
				if (isReversed)
				{
					if (animationTime <= mTime)
					{
						triggerEvent(modelName, isReversed, gameEngine);
						return true;
					}
				}
				else
				{
					if (animationTime >= mTime)
					{
						triggerEvent(modelName, isReversed, gameEngine);
						return true;
					}
				}
			}
			else
			{
				if (positionKeyFrame.mEaseType == mEaseType)
				{
					triggerEvent(modelName, isReversed, gameEngine);
					return true;
				}
			}
		}

		return false;
	}

	void triggerEvent(std::string modelName, bool isReversed, cControlGameEngine &gameEngine) 
	{
		if (isReversed)
		{
			mEventTriggered = false;

			if(mTimeBasedEvent)
				gameEngine.TurnVisibilityOn(modelName);
		}

		else
			mEventTriggered = true;

		if (!mTimeBasedEvent)
		{
			switch (mEaseType)
			{
			case EasingType::Linear:
				std::cout << std::endl;
				std::cout << modelName << " HAS NO EASE TYPE ANIMATION : COLORING OBJECT WHITE" << std::endl;
				std::cout << std::endl;
				gameEngine.ChangeColor(modelName, 1.0f, 1.0f, 1.0f);
				break;

			case EasingType::EaseIn:
				std::cout << std::endl;
				std::cout << modelName << " HAS 'EASE IN' TYPE ANIMATION : COLORING OBJECT RED" << std::endl;
				std::cout << std::endl;
				gameEngine.ChangeColor(modelName, 0.85f, 0.1f, 0.1f);
				break;

			case EasingType::EaseOut:
				std::cout << std::endl;
				std::cout << modelName << " HAS 'EASE OUT' TYPE ANIMATION : COLORING OBJECT YELLOW" << std::endl;
				std::cout << std::endl;
				gameEngine.ChangeColor(modelName, 1.0f, 1.0f, 0.0f);
				break;

			case EasingType::EaseInOut:
				std::cout << std::endl;
				std::cout << modelName << " HAS 'EASE IN OUT' TYPE ANIMATION : COLORING OBJECT GREEN" << std::endl;
				std::cout << std::endl;
				gameEngine.ChangeColor(modelName, 0.1f, 0.85f, 0.1f);
				break;
			}
		}
		else
		{
			if (!isReversed)
			{
				std::cout << std::endl;
				std::cout << modelName << " TIME BASED KEYFRAME EVENT : The" << modelName << " IS NOW DESTROYED !" << std::endl;
				std::cout << std::endl;

				gameEngine.TurnVisibilityOn(modelName);
			}
		}
	}
};	

struct sAnimation
{
	std::vector<sPositionKeyFrame> mPositionKeyFrames;
	std::vector<sScaleKeyFrame> mScaleKeyFrames;
	std::vector<sRotationKeyFrame> mRotationKeyFrames;

	std::string modelName = "";

	int mCurrentPositionKeyFrame = 0;
	double mTime;
};

class cAnimationSystem
{
public:

	std::vector<sAnimation*> mAnimationList;
	std::vector<sKeyFrameEvent*> mKeyFrameEventList;

	bool isAnimationPaused();
	bool isAnimationReversed();
	bool CheckForwardAnimationCompletion();
	bool CheckReverseAnimationCompletion();
	void PauseAnimation();
	void PlayAnimation();
	void ReverseAnimationFlow();
	void NormalAnimationFlow();
	void ChangeAnimationSpeed(float speed);
	void ResetNormalAnimationFlags();
	void ResetReverseAnimationFlags();
	void ResetAnimationTime();
	void UpdateAnimations(cControlGameEngine gameEngine);

private:

	double mOverallAnimationtime;
	float mAnimationSpeed = 1.f;
	bool mPauseAnimation = false;
	bool mReverseAnimation = false;
	bool mPositionAnimationComplete = false;
	bool mRotationAnimationComplete = false;
	bool mScaleAnimationComplete = false;
	bool mReversePositionAnimationComplete = false;
	bool mReverseRotationAnimationComplete = false;
	bool mReverseScaleAnimationComplete = false;

	void ForwardAnimation(cControlGameEngine gameEngine);
	void ReverseAnimation(cControlGameEngine gameEngine);
	float FindValueAfterEasingEffect(double startKeyFrameTime, double endKeyFrameTime, EasingType easeType);
};
