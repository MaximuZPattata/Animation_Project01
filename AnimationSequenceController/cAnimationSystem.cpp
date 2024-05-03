#include "pch.h"
#include "cAnimationSystem.h"
#include <glm/gtx/easing.hpp>

float cAnimationSystem::FindValueAfterEasingEffect(double startKeyFrameTime, double endKeyFrameTime, EasingType easeType)
{
	float percent = 0.f;

	if(mReverseAnimation)
		percent = (startKeyFrameTime - mOverallAnimationtime) / (startKeyFrameTime - endKeyFrameTime);

	else
		percent = (mOverallAnimationtime - startKeyFrameTime) / (endKeyFrameTime - startKeyFrameTime);

	float result = 0.f;

	switch (easeType)
	{
	case EasingType::Linear:
		result = percent;
		break;

	case EasingType::EaseIn:
		result = glm::sineEaseIn(percent);
		break;

	case EasingType::EaseOut:
		result = glm::sineEaseOut(percent);
		break;

	case EasingType::EaseInOut:
		result = glm::sineEaseInOut(percent);
		break;
	}

	return result;
}

void cAnimationSystem::ForwardAnimation(cControlGameEngine gameEngine)
{
	glm::vec3 modelPosition = glm::vec3(0.f);
	glm::quat modelRotation = glm::quat(0.f, 0.f, 0.f, 1.f);
	float modelScale = 1.f;

	int currentPositionKeyFrameIndex = 0;
	bool keyFrameChange = false;

	int positionFramesCompletionCount = 0;
	int rotationFramesCompletionCount = 0;
	int scaleFramesCompletionCount = 0;

	std::string modelName = "";

	for (int modelIterator = 0; modelIterator < mAnimationList.size(); modelIterator++)
	{
		modelName = mAnimationList[modelIterator]->modelName;

		//------------------------------------------------------------------------------------
		//----------------------HANDLING POSITION KEYFRAMES-----------------------------------
		//------------------------------------------------------------------------------------

		if (mAnimationList[modelIterator]->mPositionKeyFrames.size() > 0)
		{
			//--------------------If only one KeyFrame is set----------------------------------------

			if (mAnimationList[modelIterator]->mPositionKeyFrames.size() == 1)
				modelPosition = mAnimationList[modelIterator]->mPositionKeyFrames[0].mPosition;

			else
			{
				//---------------Initializing values for multiple KeyFrames set-----------------------------

				int KeyFrameEndIndex = 0;

				// Initializing the animation time to match the last keyframe time
				if (mOverallAnimationtime < mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex].mTime)
					mOverallAnimationtime = mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex].mTime;

				for (; KeyFrameEndIndex < mAnimationList[modelIterator]->mPositionKeyFrames.size(); KeyFrameEndIndex++)
				{
					if (mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex].mTime > mOverallAnimationtime)
					{
						if (mAnimationList[modelIterator]->mCurrentPositionKeyFrame != KeyFrameEndIndex)
						{
							mAnimationList[modelIterator]->mCurrentPositionKeyFrame = KeyFrameEndIndex;
							keyFrameChange = true;
						}

						break;
					}
				}

				//--------------If KeyFrame index has reached the last KeyFrame Position-------------------

				if (KeyFrameEndIndex >= mAnimationList[modelIterator]->mPositionKeyFrames.size())
				{
					modelPosition = mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex - 1].mPosition;

					keyFrameChange = true;

					positionFramesCompletionCount += 1;

					// Updating position values to the model
					gameEngine.MoveModel(modelName, modelPosition.x, modelPosition.y, modelPosition.z);
				}

				else
				{
					int KeyFrameStartIndex = KeyFrameEndIndex - 1;
					currentPositionKeyFrameIndex = KeyFrameEndIndex;

					//----------------Initializing start and end KeyFrame objects--------------------------

					sPositionKeyFrame startKeyFrame = mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameStartIndex];
					sPositionKeyFrame endKeyFrame = mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex];

					//-------------Calculating position value based on Easing effect-----------------------

					float result = FindValueAfterEasingEffect(startKeyFrame.mTime, endKeyFrame.mTime, endKeyFrame.mEaseType);

					glm::vec3 deltaPos = endKeyFrame.mPosition - startKeyFrame.mPosition;

					modelPosition = startKeyFrame.mPosition + deltaPos * result;

					//------------------Updating position values to the model-----------------------------

					gameEngine.MoveModel(modelName, modelPosition.x, modelPosition.y, modelPosition.z);
				}
			}
		}

		else
			positionFramesCompletionCount += 1;

		if (!mPositionAnimationComplete && positionFramesCompletionCount == mAnimationList.size())
			mPositionAnimationComplete = true;

		//------------------------------------------------------------------------------------
		//----------------------HANDLING ROTATION KEYFRAMES-----------------------------------
		//------------------------------------------------------------------------------------

		if (mAnimationList[modelIterator]->mRotationKeyFrames.size() > 0)
		{
			//--------------------If only one KeyFrame is set------------------------------------

			if (mAnimationList[modelIterator]->mRotationKeyFrames.size() == 1)
				modelRotation = mAnimationList[modelIterator]->mRotationKeyFrames[0].mRotation;

			else
			{
				//-------------Initializing values for multiple KeyFrames set------------------------

				int KeyFrameEndIndex = 0;

				// Initializing the animation time to match the last keyframe time
				if (mOverallAnimationtime < mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex].mTime)
					mOverallAnimationtime = mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex].mTime;

				for (; KeyFrameEndIndex < mAnimationList[modelIterator]->mRotationKeyFrames.size(); KeyFrameEndIndex++)
				{
					if (mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex].mTime > mOverallAnimationtime)
						break;
				}

				//--------------If KeyFrame index has reached the last KeyFrame Position-------------------

				if (KeyFrameEndIndex >= mAnimationList[modelIterator]->mRotationKeyFrames.size())
				{
					modelRotation = mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex - 1].mRotation;

					rotationFramesCompletionCount += 1;

					// Updating position values to the model
					gameEngine.RotateMeshModelUsingQuaternion(modelName, modelRotation);
				}

				else
				{
					int KeyFrameStartIndex = KeyFrameEndIndex - 1;

					//----------------Initializing start and end KeyFrame objects--------------------------

					sRotationKeyFrame startKeyFrame = mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameStartIndex];
					sRotationKeyFrame endKeyFrame = mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex];

					//-------------Calculating rotation value based on Easing effect-----------------------

					float result = FindValueAfterEasingEffect(startKeyFrame.mTime, endKeyFrame.mTime, endKeyFrame.mEaseType);

					float startKeyFrameRadianVal = glm::radians(startKeyFrame.mAngleDegrees);
					float endKeyFrameRadianVal = glm::radians(endKeyFrame.mAngleDegrees);

					glm::quat startKeyFrameRotationQuaternion = glm::angleAxis(startKeyFrameRadianVal, glm::vec3(startKeyFrame.mRotation.x,
						startKeyFrame.mRotation.y, startKeyFrame.mRotation.z));
					glm::quat endKeyFrameRotationQuaternion = glm::angleAxis(endKeyFrameRadianVal, glm::vec3(endKeyFrame.mRotation.x,
						endKeyFrame.mRotation.y, endKeyFrame.mRotation.z));

					modelRotation = glm::slerp(startKeyFrameRotationQuaternion, endKeyFrameRotationQuaternion, result);

					//------------------Updating rotation values to the model-------------------------------

					gameEngine.RotateMeshModelUsingQuaternion(modelName, modelRotation);
				}
			}
		}

		else
			rotationFramesCompletionCount += 1;

		if (!mRotationAnimationComplete && rotationFramesCompletionCount == mAnimationList.size())
			mRotationAnimationComplete = true;

		//------------------------------------------------------------------------------------
		//----------------------HANDLING SCALE KEYFRAMES--------------------------------------
		//------------------------------------------------------------------------------------

		if (mAnimationList[modelIterator]->mScaleKeyFrames.size() > 0)
		{
			//--------------------If only one KeyFrame is set----------------------------------------

			if (mAnimationList[modelIterator]->mScaleKeyFrames.size() == 1)
				modelScale = mAnimationList[modelIterator]->mScaleKeyFrames[0].mScale;

			else
			{
				//----------Initializing values for multiple KeyFrames set----------------------------

				int KeyFrameEndIndex = 0;

				// Initializing the animation time to match the last keyframe time
				if (mAnimationList[modelIterator]->mPositionKeyFrames.size() > 0)
				{
					if (mOverallAnimationtime < mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex].mTime)
						mOverallAnimationtime = mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex].mTime;
				}

				for (; KeyFrameEndIndex < mAnimationList[modelIterator]->mScaleKeyFrames.size(); KeyFrameEndIndex++)
				{
					if (mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameEndIndex].mTime > mOverallAnimationtime)
						break;
				}

				//--------If KeyFrame index has reached the last KeyFrame Position--------------------

				if (KeyFrameEndIndex >= mAnimationList[modelIterator]->mScaleKeyFrames.size())
				{
					modelScale = mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameEndIndex - 1].mScale;

					scaleFramesCompletionCount += 1;

					// Updating scale value to the model
					gameEngine.ScaleModel(modelName, modelScale);
				}

				else
				{
					int KeyFrameStartIndex = KeyFrameEndIndex - 1;

					//----------------Initializing start and end KeyFrame objects--------------------------

					sScaleKeyFrame startKeyFrame = mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameStartIndex];
					sScaleKeyFrame endKeyFrame = mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameEndIndex];

					//----------------Calculating scale value based on Easing effect-----------------------

					float result = FindValueAfterEasingEffect(startKeyFrame.mTime, endKeyFrame.mTime, endKeyFrame.mEaseType);

					float deltaScale = endKeyFrame.mScale - startKeyFrame.mScale;

					modelScale = startKeyFrame.mScale + deltaScale * result;

					//----------------------Updating scale value to the model------------------------------

					gameEngine.ScaleModel(modelName, modelScale);
				}
			}
		}

		else
			scaleFramesCompletionCount += 1;

		if (!mScaleAnimationComplete && scaleFramesCompletionCount == mAnimationList.size())
			mScaleAnimationComplete = true;

		//------------------------------------------------------------------------------------
		//----------------------HANDLING KEYFRAME EVENTS--------------------------------------
		//------------------------------------------------------------------------------------

		if (keyFrameChange)
		{
			for (int eventIterator = 0; eventIterator < mKeyFrameEventList.size(); eventIterator++)
			{
				if (!mKeyFrameEventList[eventIterator]->mEventTriggered)
				{
					if (currentPositionKeyFrameIndex < mAnimationList[modelIterator]->mPositionKeyFrames.size())
						if (mKeyFrameEventList[eventIterator]->checkForEventTrigger(modelName, mOverallAnimationtime, 
							mAnimationList[modelIterator]->mPositionKeyFrames[currentPositionKeyFrameIndex], false, gameEngine))
							break;
				}
			}
		}
	}
}

void cAnimationSystem::ReverseAnimation(cControlGameEngine gameEngine)
{
	glm::vec3 modelPosition = glm::vec3(0.f);
	glm::quat modelRotation = glm::quat(0.f, 0.f, 0.f, 1.f);
	float modelScale = 1.f;

	int currentPositionKeyFrameIndex = 0;

	int positionFramesCompletionCount = 0;
	int rotationFramesCompletionCount = 0;
	int scaleFramesCompletionCount = 0;

	bool keyFrameChange = false;

	std::string modelName = "";

	for (int modelIterator = 0; modelIterator < mAnimationList.size(); modelIterator++)
	{
		modelName = mAnimationList[modelIterator]->modelName;

		//------------------------------------------------------------------------------------
		//----------------------HANDLING POSITION KEYFRAMES-----------------------------------
		//------------------------------------------------------------------------------------

		if (mAnimationList[modelIterator]->mPositionKeyFrames.size() > 0)
		{
			//--------------------If only one KeyFrame is set----------------------------------------

			if (mAnimationList[modelIterator]->mPositionKeyFrames.size() == 1)
				modelPosition = mAnimationList[modelIterator]->mPositionKeyFrames[0].mPosition;

			else
			{
				//---------------Initializing values for multiple KeyFrames set-----------------------------

				int KeyFrameEndIndex = mAnimationList[modelIterator]->mPositionKeyFrames.size() - 1;

				// Waiting for the animation time to match the last keyframe time
				if (mOverallAnimationtime < mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex].mTime)
				{
					for (; KeyFrameEndIndex >= 0; KeyFrameEndIndex--)
					{
						if (mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex].mTime < mOverallAnimationtime)
						{
							if (mAnimationList[modelIterator]->mCurrentPositionKeyFrame != KeyFrameEndIndex)
							{
								mAnimationList[modelIterator]->mCurrentPositionKeyFrame = KeyFrameEndIndex;
								keyFrameChange = true;
							}

							break;
						}
					}

					//--------------If KeyFrame index has reached the last KeyFrame Position-------------------

					if (KeyFrameEndIndex < 0)
					{
						modelPosition = mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex + 1].mPosition;

						positionFramesCompletionCount += 1;

						// Updating position values to the model
						gameEngine.MoveModel(modelName, modelPosition.x, modelPosition.y, modelPosition.z);
					}

					else
					{
						int KeyFrameStartIndex = KeyFrameEndIndex + 1;
						currentPositionKeyFrameIndex = KeyFrameStartIndex;

						//----------------Initializing start and end KeyFrame objects--------------------------

						sPositionKeyFrame startKeyFrame = mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameStartIndex];
						sPositionKeyFrame endKeyFrame = mAnimationList[modelIterator]->mPositionKeyFrames[KeyFrameEndIndex];

						//-------------Calculating position value based on Easing effect-----------------------

						float result = FindValueAfterEasingEffect(startKeyFrame.mTime, endKeyFrame.mTime, startKeyFrame.mEaseType);

						glm::vec3 deltaPos = startKeyFrame.mPosition - endKeyFrame.mPosition;

						modelPosition = startKeyFrame.mPosition - deltaPos * result;

						//------------------Updating position values to the model-----------------------------

						gameEngine.MoveModel(modelName, modelPosition.x, modelPosition.y, modelPosition.z);
					}
				}
			}
		}

		else
			positionFramesCompletionCount += 1;

		if(!mReversePositionAnimationComplete && positionFramesCompletionCount == mAnimationList.size())
			mReversePositionAnimationComplete = true;

		//------------------------------------------------------------------------------------
		//----------------------HANDLING ROTATION KEYFRAMES-----------------------------------
		//------------------------------------------------------------------------------------

		if (mAnimationList[modelIterator]->mRotationKeyFrames.size() > 0)
		{
			//--------------------If only one KeyFrame is set------------------------------------

			if (mAnimationList[modelIterator]->mRotationKeyFrames.size() == 1)
				modelRotation = mAnimationList[modelIterator]->mRotationKeyFrames[0].mRotation;

			else
			{
				//-------------Initializing values for multiple KeyFrames set------------------------

				int KeyFrameEndIndex = mAnimationList[modelIterator]->mRotationKeyFrames.size() - 1;

				// Waiting for the animation time to match the last keyframe time
				if (mOverallAnimationtime < mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex].mTime)
				{

					for (; KeyFrameEndIndex >= 0; KeyFrameEndIndex--)
					{
						if (mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex].mTime < mOverallAnimationtime)
							break;
					}

					//--------------If KeyFrame index has reached the last KeyFrame Position-------------------

					if (KeyFrameEndIndex < 0)
					{
						modelRotation = mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex + 1].mRotation;

						rotationFramesCompletionCount += 1;

						// Updating position values to the model
						gameEngine.RotateMeshModelUsingQuaternion(modelName, modelRotation);

					}

					else
					{
						int KeyFrameStartIndex = KeyFrameEndIndex + 1;

						//----------------Initializing start and end KeyFrame objects--------------------------

						sRotationKeyFrame startKeyFrame = mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameStartIndex];
						sRotationKeyFrame endKeyFrame = mAnimationList[modelIterator]->mRotationKeyFrames[KeyFrameEndIndex];

						//-------------Calculating rotation value based on Easing effect-----------------------

						float result = FindValueAfterEasingEffect(startKeyFrame.mTime, endKeyFrame.mTime, startKeyFrame.mEaseType);

						float startKeyFrameRadianVal = glm::radians(startKeyFrame.mAngleDegrees);
						float endKeyFrameRadianVal = glm::radians(endKeyFrame.mAngleDegrees);

						glm::quat startKeyFrameRotationQuaternion = glm::angleAxis(startKeyFrameRadianVal, glm::vec3(startKeyFrame.mRotation.x,
							startKeyFrame.mRotation.y, startKeyFrame.mRotation.z));
						glm::quat endKeyFrameRotationQuaternion = glm::angleAxis(endKeyFrameRadianVal, glm::vec3(endKeyFrame.mRotation.x,
							endKeyFrame.mRotation.y, endKeyFrame.mRotation.z));

						modelRotation = glm::slerp(startKeyFrameRotationQuaternion, endKeyFrameRotationQuaternion, result);

						//------------------Updating rotation values to the model-------------------------------

						gameEngine.RotateMeshModelUsingQuaternion(modelName, modelRotation);
					}
				}
			}
		}

		else
			rotationFramesCompletionCount += 1;

		if (!mReverseRotationAnimationComplete && rotationFramesCompletionCount == mAnimationList.size())
			mReverseRotationAnimationComplete = true;

		//------------------------------------------------------------------------------------
		//----------------------HANDLING SCALE KEYFRAMES--------------------------------------
		//------------------------------------------------------------------------------------

		if (mAnimationList[modelIterator]->mScaleKeyFrames.size() > 0)
		{
			//--------------------If only one KeyFrame is set----------------------------------------

			if (mAnimationList[modelIterator]->mScaleKeyFrames.size() == 1)
				modelScale = mAnimationList[modelIterator]->mScaleKeyFrames[0].mScale;

			else
			{
				//----------Initializing values for multiple KeyFrames set----------------------------

				int KeyFrameEndIndex = mAnimationList[modelIterator]->mScaleKeyFrames.size() - 1;

				// Waiting for the animation time to match the last keyframe time
				if (mOverallAnimationtime < mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameEndIndex].mTime)
				{
					for (; KeyFrameEndIndex >= 0; KeyFrameEndIndex--)
					{
						if (mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameEndIndex].mTime < mOverallAnimationtime)
							break;
					}

					//--------If KeyFrame index has reached the last KeyFrame Position--------------------

					if (KeyFrameEndIndex < 0)
					{
						modelScale = mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameEndIndex + 1].mScale;

						scaleFramesCompletionCount += 1;

						// Updating scale value to the model
						gameEngine.ScaleModel(modelName, modelScale);
					}

					else
					{
						int KeyFrameStartIndex = KeyFrameEndIndex + 1;

						//----------------Initializing start and end KeyFrame objects--------------------------

						sScaleKeyFrame startKeyFrame = mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameStartIndex];
						sScaleKeyFrame endKeyFrame = mAnimationList[modelIterator]->mScaleKeyFrames[KeyFrameEndIndex];

						//----------------Calculating scale value based on Easing effect-----------------------

						float result = FindValueAfterEasingEffect(startKeyFrame.mTime, endKeyFrame.mTime, startKeyFrame.mEaseType);

						float deltaScale = startKeyFrame.mScale - endKeyFrame.mScale;

						modelScale = startKeyFrame.mScale - deltaScale * result;

						//----------------------Updating scale value to the model------------------------------

						gameEngine.ScaleModel(modelName, modelScale);
					}
				}
			}
	
		}

		else
			scaleFramesCompletionCount += 1;

		if (!mReverseScaleAnimationComplete && scaleFramesCompletionCount == mAnimationList.size())
			mReverseScaleAnimationComplete = true;

		//------------------------------------------------------------------------------------
		//----------------------HANDLING KEYFRAME EVENTS--------------------------------------
		//------------------------------------------------------------------------------------

		if (keyFrameChange)
		{
			for (int eventIterator = 0; eventIterator < mKeyFrameEventList.size(); eventIterator++)
			{
				if (mKeyFrameEventList[eventIterator]->mEventTriggered)
				{
					if (currentPositionKeyFrameIndex >= 0 && mAnimationList[modelIterator]->mPositionKeyFrames.size() >=currentPositionKeyFrameIndex)
						if (mKeyFrameEventList[eventIterator]->checkForEventTrigger(modelName, mOverallAnimationtime,
							mAnimationList[modelIterator]->mPositionKeyFrames[currentPositionKeyFrameIndex], true, gameEngine))
							break;
				}
			}
		}
	}
}

void cAnimationSystem::UpdateAnimations(cControlGameEngine gameEngine)
{
	if (!mPauseAnimation)
	{
		if (mReverseAnimation)
		{
			if (!mReversePositionAnimationComplete || !mReverseRotationAnimationComplete || !mReverseScaleAnimationComplete)
			{
				// Decrementing animation time with deltaTime
				mOverallAnimationtime -= gameEngine.deltaTime;
				ReverseAnimation(gameEngine);
			}
		}
		else
		{
			if (!mPositionAnimationComplete || !mRotationAnimationComplete || !mScaleAnimationComplete)
			{
				// Incrementing animation time with deltaTime
				mOverallAnimationtime += gameEngine.deltaTime * mAnimationSpeed;

				ForwardAnimation(gameEngine);
			}
			else
				return;
		}
	}
}

bool cAnimationSystem::isAnimationPaused()
{
	return mPauseAnimation;
}

bool cAnimationSystem::isAnimationReversed()
{
	return mReverseAnimation;
}

void cAnimationSystem::PauseAnimation()
{
	mPauseAnimation = true;
}

void cAnimationSystem::PlayAnimation()
{
	mPauseAnimation = false;
}

void cAnimationSystem::ReverseAnimationFlow()
{
	if (!mReverseAnimation)
	{
		mAnimationSpeed = 1.f;
		mReverseAnimation = true;
	}
}

void cAnimationSystem::NormalAnimationFlow()
{
	if (mReverseAnimation)
	{
		mAnimationSpeed = 1.f;
		mReverseAnimation = false;
	}
}

void cAnimationSystem::ChangeAnimationSpeed(float speed)
{
	mAnimationSpeed = speed;
}

bool cAnimationSystem::CheckForwardAnimationCompletion()
{
	return (mPositionAnimationComplete && mRotationAnimationComplete && mScaleAnimationComplete);
}

bool cAnimationSystem::CheckReverseAnimationCompletion()
{
	return (mReversePositionAnimationComplete && mReverseRotationAnimationComplete && mReverseScaleAnimationComplete);
}

void cAnimationSystem::ResetNormalAnimationFlags()
{
	mPositionAnimationComplete = false;
	mRotationAnimationComplete = false;
	mScaleAnimationComplete = false;
}

void cAnimationSystem::ResetReverseAnimationFlags()
{
	mReversePositionAnimationComplete = false;
	mReverseRotationAnimationComplete = false;
	mReverseScaleAnimationComplete = false;
}

void cAnimationSystem::ResetAnimationTime()
{
	mOverallAnimationtime = 0.0f;
}