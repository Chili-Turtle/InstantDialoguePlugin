# InstantDialoguePlugin Alpha 0.1.0

## What is Instant Dialogue?
 Instant Dialogue is a visual dialogue tree editor for UE4
 
## How to install
1. Create a Plugins folder in your project, and copy the plugin in to the folder.
2. Your project has to be a c++ project otherwise you can't package your project

https://user-images.githubusercontent.com/43949113/143775614-15b26310-d553-4f3a-b134-c1af7e04b85b.mp4

## Quick Guid
Download the example project here https://github.com/Chili-Turtle/InstantDialogueUE4
https://user-images.githubusercontent.com/43949113/144940791-7471435b-0890-46aa-b956-e7f473c3ebd9.mp4

## Roadmap
- Make the portrait a Button (so you can change the portrait on the TextNode)
- Localization
- Blackboard like varialbles to use in text
- Richtext Icons
- Updating Nodes when setting Portrait/Text (Disabled because it threw an error when packedging)
- Remove the weired gloss from  the nodes

## FAQ
1. I already have a blueprint project WICH IS NOT A C++ project, can I still use this plugin?
- Yes, In your project go to "File" header on the left top press "New C++ Class", create a new c++ actor and call it something like "DummyActor". Wait until the project compiles and your are done.

https://user-images.githubusercontent.com/43949113/143776994-f36d6e31-4848-4e9a-b892-2874ab12837b.mp4
