<<--------------------------------------------------------------------------------Animation_Project_01---------------------------------------------------------------------------------------------------------->>

# Developers :
	- This project was done by Max Sherwin

# About Project :
	- The project reads the scene contents( i.e the model properties, light properties, etc) from the json file located in "BasicAnimationSequenceController->AnimationSequenceController->SceneDescription.json".

	- In the scene I have created 2 animation sequences where the "Falcon" spacecraft from StarWars will dodge through the bunch of Beholders floating in space using basic animations like position, rotation and scaling animations.
	
	- In the first scene the "Falcon" rotates and dodges a Beholder and in the second sequence, the "Falcon" hits a Beholder and they both explode in space( Shown as a sphere scaled up and down in time).
	
	- I have also added KeyFrame events to turn the color of the ship when doing a easetype effect in the sequence.
	
	- I have also implemented a time-based event keyframe. In sequence_02 when the "Falcon" explodes it is supposed to dissapear based on the time given when setting up the sequence.
	
	- Animation speeds can be changed using the number keys(1-5).

	- Spacebar can be used to pause/resume the animation anytime;

	- Animations can be viewed in the reverse order in this project.

## How to build and run : 
	- The solution "Animation_Project01.sln" has to be built(either in debug or release platform mode) and then once succeeded(probably will!), make the "AnimationSequenceController" project as the startup project and now it'll be able to run for some visual treat. (
	- When building the solution, the "MaxGameEngine" project should be run first since it is a static project without which the project with contents(AnimationSequenceController) cannot be run.
	
### Controls :
	- Up arrow key and Down arrow key = The buttons to control the normal and reverse modes for an animation sequence respectively.
	- Left arrow key and Right arrow key = Left key runs the sequence_01 animations and Right key runs the sequence_02 animations.
	- 1, 2, 3, 4 and 5 keys are pressed to change the animation speed.
	- Spacebar is pressed to pause the animation

#### Limitations to the project : 
	- The reverse is not quite perfected, still working on it. 
	- The keyframe event also is giving some trouble in a few test cases, will also need more time on that to figure out the issue. For example, when playing sequence_02 and then when reversed and sequence_02 is played again, there is a time based keyFrame that is supposed to go off which it doesnt.