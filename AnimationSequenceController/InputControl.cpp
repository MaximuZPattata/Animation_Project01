#include "pch.h"

#include "cControlGameEngine.h"
#include "cAnimationSystem.h"

extern cControlGameEngine gameEngine;
extern cAnimationSystem animationManager;

const float POSITION_OFFSET = 1.0f;

cMesh* controlMeshModel;

extern void Scene01Sequence(cAnimationSystem& animationManager);
extern void Scene02Sequence(cAnimationSystem& animationManager);

bool scene01Playing = false;
bool scene02Playing = false;

void resetCamAttributes()
{
    if (!gameEngine.mouseMoved)
        gameEngine.mouseMoved = true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    //-------------------Controls for Camera-----------------------------------------------------------

    if (mods == 0)
    {
        //----------------------------Animation Controls---------------------------------------------

        if (key == GLFW_KEY_SPACE && action) // Pause Animation
        {
            if (animationManager.isAnimationPaused())
            {
                std::cout << std::endl;
                std::cout << "ANIMATION HAS BEEN RESUMED !" << std::endl;
                std::cout << std::endl;

                animationManager.PlayAnimation();
            }
            else
            {
                std::cout << std::endl;
                std::cout << "ANIMATION HAS BEEN PAUSED !" << std::endl;
                std::cout << std::endl;

                animationManager.PauseAnimation();
            }
        }

        if (key == GLFW_KEY_LEFT)
        {
            if (!scene01Playing)
            {
                Scene01Sequence(animationManager);
                scene01Playing = true;
                scene02Playing = false;
            }
        }
        if (key == GLFW_KEY_RIGHT)
        {
            if (!scene02Playing)
            {
                Scene02Sequence(animationManager);
                scene01Playing = false;
                scene02Playing = true;
            }
        }
        if (key == GLFW_KEY_UP)
        {
            if (animationManager.isAnimationReversed() && animationManager.CheckForwardAnimationCompletion())
            {
                animationManager.ResetNormalAnimationFlags();
            }

            animationManager.NormalAnimationFlow();
        }
        if (key == GLFW_KEY_DOWN)
        {
            if (!animationManager.isAnimationReversed() && animationManager.CheckReverseAnimationCompletion())
            {
                animationManager.ResetReverseAnimationFlags();
            }

            animationManager.ReverseAnimationFlow();
        }

        if (key == GLFW_KEY_1 && action)
        {
            animationManager.ChangeAnimationSpeed(1.f);
        }
        if (key == GLFW_KEY_2 && action)
        {
            animationManager.ChangeAnimationSpeed(2.f);
        }
        if (key == GLFW_KEY_3 && action)
        {
            animationManager.ChangeAnimationSpeed(3.f);
        }
        if (key == GLFW_KEY_4 && action)
        {
            animationManager.ChangeAnimationSpeed(4.f);
        }
        if (key == GLFW_KEY_5 && action)
        {
            animationManager.ChangeAnimationSpeed(5.f);
        }
    }

    return;
}
